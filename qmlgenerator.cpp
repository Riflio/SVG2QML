#include "qmlgenerator.h"
#include <QRegularExpression>
#include <QDebug>

QMLGenerator::QMLGenerator(QObject *parent) : SVGGenerator(parent)
{

}

void QMLGenerator::setSettings(QMLGenerator::TSettings settings)
{
    _settings = settings;
}

QMLGenerator::TSettings QMLGenerator::settings() const
{
    return _settings;
}

SVGGenerator::GenerateStatus QMLGenerator::generateQML(QIODevice *device, CPrimitive *rootItm, const CDefs &defs)
{
    if ( !device->isOpen() ) { qWarning()<<"Device not opened!"; return GS_NOFILE; }

    _rootItm = rootItm;
    _defs = defs;

    _lvl = 0; //-- Для отступов
    _qml.setDevice(device);

    _qml<<"import QtQuick "<<Qt::endl;
    _qml<<"import QtQuick.Shapes"<<Qt::endl;
    _qml<<Qt::endl;

    writeStartLvl("Item");
        writePropVal("id", _settings.rootName);
        writePropVal("property var scaleShape", "Qt.size(1, 1)");
        writePropVal("property bool thinkLines", false);

        makeElement(rootItm);

    writeEndLvl();

    return GS_OK;
}

/**
* @brief Делаем отступ
* @param c
* @return
*/
QString QMLGenerator::tab(int c)
{
    QString s;
    if ( c<=0 ) return s;

    s.fill('\t', c);

    return s;
}

/**
* @brief Убираем/заменяем из ID лишние символы
* @param id
* @return
*/
QString QMLGenerator::sanitizeID(QString id)
{
    if ( id.isEmpty() ) { return id; }

    id = id.replace('-', '_').replace(' ', '_');
    id.replace(0, 1, id.left(1).toLower());

    QRegularExpressionMatch rx = QRegularExpression(QRegularExpression::anchoredPattern("([a-zA-Z0-9_])+")).match(id);

    if ( !rx.hasMatch() ) { return ""; }

    return id;
}

/**
* @brief Генерируем команды для path.d из примитивов
* @param p
* @param offset
* @return
*/
QString QMLGenerator::primitiveToPathCommands(CPrimitive *p, double offset, CMatrix transforms)
{
    CPath * tPath = nullptr;

    if ( p->type()==CPrimitive::PT_CIRCLE ) {
        CCircle * circle = static_cast<CCircle*>(p);
        circle->toPath();
        tPath = static_cast<CPath*>(circle->down);
    } else
    if ( p->type()==CPrimitive::PT_PATH ) {
        CPath * path = static_cast<CPath*>(p);
        tPath = path;
    } else
    if ( p->type()==CPrimitive::PT_RECT ) {
        CRect * rect = static_cast<CRect*>(p);
        rect->toPath();
        tPath = static_cast<CPath*>(rect->down);
    } else
    if ( p->type()==CPrimitive::PT_POLYLINE ) {
        CPolyline * polyline = static_cast<CPolyline*>(p);
        polyline->toPath();
        tPath = static_cast<CPath*>(polyline->down);
    } else
    if ( p->type()==CPrimitive::PT_POLYGON ) {
        CPolygon * polygon = static_cast<CPolygon*>(p);
        polygon->toPath();
        tPath = static_cast<CPath*>(polygon->down);
    } else
    if ( p->type()==CPrimitive::PT_LINE ) {
        CLine * line = static_cast<CLine*>(p);
        line->toPath();
        tPath = static_cast<CPath*>(line->down);
    } else
    if ( p->type()==CPrimitive::PT_ELLIPSE ) {
        CEllipse * ellipse = static_cast<CEllipse*>(p);
        ellipse->toPath();
        tPath = static_cast<CPath*>(ellipse->down);
    } else {
        qWarning()<<"Unsupported SVG element"<<p->type();
        return "";
    }

    if ( offset>0 ) { tPath = tPath->makeOffset(offset); }

    CPrimitive * tPathCopy = tPath->copyNesteed();

    if ( !transforms.isIdentity() ) {
        tPathCopy->transform().toIdentity();
        tPathCopy->applyTransform(transforms);
    }

    QString pathCommnads = generatePath(tPathCopy);

    return pathCommnads;
}

/**
* @brief Разбираемся заливкой
* @param itm
* @param isSimple - если просо цвет или градиент без трансформации
* @return
*/
bool QMLGenerator::makeFill(CPrimitive *itm, bool isSimple)
{
    if ( !itm->styles().has("fill") ) {
        writePropThinkLinesVal("fillColor", "transparent", "#000000", true);
        return true;
    }

    QVariant fill = itm->styles().get("fill");

    if ( fill.typeId()==QMetaType::QColor ) {
        QColor color = fill.value<QColor>();
        if ( itm->styles().has("fill-opacity") ) { color.setAlphaF(itm->styles().get("fill-opacity").value<CSS::MeasureUnit>().val());  }
        writePropThinkLinesVal("fillColor", "transparent", color.name(QColor::HexArgb), true);
        return true;
    } else
    if ( fill.typeId()==QMetaType::QUrl ) {
        if ( CDefs::isCDefLink(fill.toUrl()) ) {
            CDef * def = _defs.get(fill.toUrl());

            if ( def==nullptr ) {
                qWarning()<<"Unable find def"<<fill.toUrl();
                return false;
            }

            if ( def->defType()==CDef::DF_LINEARGRADIENT || def->defType()==CDef::DF_RADIALGRADIENT) {
                FGradient * gr = dynamic_cast<FGradient*>(def);
                if ( gr->transform().isIdentity() ) {  //-- Если нет трансформаций, то выводим как обычно
                    makeFillGradient(itm, gr, def->defType());
                    return true;
                } else { //-- Есть трансформации - придётся мучаться
                    if ( isSimple ) { //-- Если режим простая заливка, то выведем просто заливку цветом
                        writePropVal("fillColor", gr->stops().last().color.name(QColor::HexArgb), true);
                        return false;
                    } else {
                        makeFillGradientTransform(itm, gr, def->defType());
                        return true;
                    }
                }
            } else {
                qWarning()<<"Unsupported fill type"<<fill.toString();
            }
        } else {
            qWarning()<<"Unsupported fill value url"<<fill.toString();
        }
    } else
    if ( fill.typeId()==QMetaType::QString ) {
        if ( fill.toString()=="none" ) {            
            writePropVal("fillColor", "transparent", true);
            return true;
        }
    } else {
        qWarning()<<"Unsupported fill type"<<fill.toString();
    }

    return false;
}

/**
* @brief Разбираемся с прозрачностью
* @param itm
* @return
*/
bool QMLGenerator::makeOpacity(CPrimitive *itm)
{
    if ( itm->styles().has("opacity") ) {
        double opacity = itm->styles().get("opacity").value<CSS::MeasureUnit>().val();
        _qml<<tab(_lvl)<<"opacity: "<<opacity<<Qt::endl;
    }
    return true;
}

/**
* @brief Выводим градиент без трансформаций
* @param itm
* @param gr
* @param type
*/
void QMLGenerator::makeFillGradient(CPrimitive *itm, FGradient *gr, CDef::TDefType type)
{
    Q_UNUSED(itm);
    if ( type==CDef::DF_LINEARGRADIENT ) { makeLinearGradient(dynamic_cast<FLinearGradient*>(gr)); } else
    if ( type==CDef::DF_RADIALGRADIENT ) { makeRadialGradient(dynamic_cast<FRadialGradient*>(gr)); }
}

/**
* @brief Makefill if has gradientTransform
* @param itm
* @param gr
* @param type
*/
void QMLGenerator::makeFillGradientTransform(CPrimitive *itm, FGradient *gr, CDef::TDefType type)
{
    //-- Внутрь основной засовываем итем, внутрь фигуру (PathLine - rect для line gradient или PathAngleArc - circle) с заливкой градиентом, к итему ещё применяем маску, что бы градиент не вылезал за его пределы

    writeStartLvl("Item");
        writePropElVal("width", _settings.rootName, "width");
        writePropElVal("height", _settings.rootName, "height");
        writeStartLvl("Shape");
            writePropElVal("width", _settings.rootName, "width");
            writePropElVal("height", _settings.rootName, "height");

            //-- Path
            writeStartLvl("ShapePath");
                if ( _settings.enableScale ) { writePropElVal("scale", _settings.rootName, "scaleShape"); }

                if ( type==CDef::DF_LINEARGRADIENT ) {
                    //-- Делаем строго горизонтальным с началом x1y1, а поворот между x1y1 и x2y2 добавляем к матрице трансформации что бы фигура была по оси x1y1 x2y2
                    FLinearGradient * lg = dynamic_cast<FLinearGradient*>(gr);

                    double l = lg->startPoint().lengthTo(lg->endPoint()); //-- Gradient length
                    double w = 100; //-- Gradient continuation before first and after last stop points
                    double h = 100; //TODO: As max(width, height) of bouding box?

                    writePropVal("startX", lg->startPoint().x()-w);
                    writePropVal("startY", lg->startPoint().y()-h);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", lg->startPoint().x()+l+w, false, true);
                        writePropVal("y", lg->startPoint().y()-h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", lg->startPoint().x()+l+w, false, true);
                        writePropVal("y", lg->startPoint().y()+h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", lg->startPoint().x()-w, false, true);
                        writePropVal("y", lg->startPoint().y()+h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", lg->startPoint().x()-w, false, true);
                        writePropVal("y", lg->startPoint().y()-h, false, true);
                    writeEndLvl(true);

                    double rotation = lg->startPoint().angle(lg->startPoint()-CPoint(0.0,1.0), lg->endPoint());

                    CMatrix transformMatrix = CMatrix::identity(3,3);
                    transformMatrix.multiplication(gr->transform());

                    CPoint originPoint = lg->startPoint();
                    originPoint.transform(transformMatrix);

                    CMatrix rotM = CMatrix::identity(3, 3);
                    rotM.translate(originPoint.x(), originPoint.y());
                    rotM.rotate(rotation-M_PI_2);
                    rotM.translate(-originPoint.x(), -originPoint.y());

                    rotM.multiplication(transformMatrix);

                    gr->setTransform(rotM);

                    lg->setStartPoint(CPoint(lg->startPoint().x(), lg->startPoint().y()));
                    lg->setEndPoint(CPoint(lg->startPoint().x()+l, lg->startPoint().y()));
                } else
                if ( type==CDef::DF_RADIALGRADIENT ) {
                    FRadialGradient * rg = dynamic_cast<FRadialGradient*>(gr);
                    //-- Делаем обычную окружность с радиентом, потом трансформациями доводим до кондиции

                    double rr = 100; //-- Gradient continuation

                    writeStartLvl("PathAngleArc");
                        writePropVal("centerX", rg->centerPoint().x());
                        writePropVal("centerY", rg->centerPoint().y());
                        writePropVal("radiusX", rg->radius()+rr);
                        writePropVal("radiusY", rg->radius()+rr);
                        writePropVal("startAngle", 0);
                        writePropVal("sweepAngle", 360);
                    writeEndLvl();
                }

                writePropVal("strokeWidth", 0);
                writePropVal("strokeColor", "transparent", true);

                makeFillGradient(itm, gr, type);

            writeEndLvl();


            //-- Трансформации
            CMatrix m = gr->transform();
            makeTransform(m);

        writeEndLvl();

        //-- Ещё и clipPath придётся сделать, что бы фигура с внутренним градиентом не вылезала шибко далеко, но контур придётся сделать меньше на половину ширины заливки
        writePropVal("layer.enabled", true);
        writeStartLvlProp("layer.effect", "ShaderEffect");
            QString shapeGrID = QString("%1_grcl").arg(sanitizeID(itm->ID()));
            writeStartLvl("Shape");
                writePropVal("id", shapeGrID);
                writePropVal("visible", false);
                writePropVal("layer.enabled", true);
                writePropElVal("width", _settings.rootName, "width");
                writePropElVal("height", _settings.rootName, "height");

                writeStartLvl("ShapePath");
                    if ( _settings.enableScale ) { writePropElVal("scale", _settings.rootName, "scaleShape"); }
                    writeStartLvl("PathSvg");

                        //-- In negative coordinatese figure is not drawn, so we must first move
                        QRectF bb = itm->getBBox();
                        CMatrix bbTr = CMatrix::identity(3, 3).translate(-bb.left(), -bb.top());

                        QString pathCommands = primitiveToPathCommands(itm, 0, bbTr);

                        writePropVal("path", pathCommands, true);
                    writeEndLvl();
                    writePropVal("fillColor", "#000000", true);
                    writePropVal("strokeWidth", 0);
                    writePropVal("strokeColor", "transparent", true);
                writeEndLvl();

                makeTransform(CMatrix::identity(3, 3).translate(bb.left(), bb.top()));

            writeEndLvl();

            writePropVal("fragmentShader", "qrc:/mask.frag.qsb", true);
            writePropVal("property var maskSource", shapeGrID);

        writeEndLvl();


    writeEndLvl();
}

/**
* @brief Делаем заливку радиальным градиентом
* @param gr
*/
void QMLGenerator::makeRadialGradient(FRadialGradient *gr)
{    
    CPoint cp = gr->centerPoint();
    CPoint fp = gr->focalPoint();

    writeStartLvlProp("fillGradient", "RadialGradient");
        writePropVal("centerX", cp.x());
        writePropVal("centerY", cp.y());
        writePropVal("focalX", (fp.isEmpty())? "centerX" : QString::number(fp.x()));
        writePropVal("focalY", (fp.isEmpty())? "centerY" : QString::number(fp.y()));
        writePropVal("centerRadius", gr->radius());
        writePropVal("focalRadius", gr->focalRadius());
        makeGradientStops(gr);
    writeEndLvl();
}

/**
* @brief Делаем заливку линейным градиентом
* @param gr
* @param rootID
*/
void QMLGenerator::makeLinearGradient(FLinearGradient *gr)
{
    CPoint sp = gr->startPoint();
    CPoint ep = gr->endPoint();

    writeStartLvlProp("fillGradient", "LinearGradient");
        writePropVal("x1", sp.x());
        writePropVal("y1", sp.y());
        writePropVal("x2", ep.x());
        writePropVal("y2", ep.y());
        makeGradientStops(gr);
    writeEndLvl();
}

/**
* @brief Разбираемся с обводкой
* @param itm
*/
void QMLGenerator::makeStroke(CPrimitive *itm)
{
    if ( itm->styles().has("stroke") ) {
        QVariant stroke = itm->styles().get("stroke");
        if ( stroke.typeId()==QMetaType::QColor ) {
            QColor color = stroke.value<QColor>();
            if ( itm->styles().has("stroke-opacity") ) { color.setAlphaF(itm->styles().get("stroke-opacity").value<CSS::MeasureUnit>().val()); }
            writePropThinkLinesVal("strokeColor", "#000000", color.name(QColor::HexArgb), true);
        } else
        if ( stroke.toString()=="none" || stroke.toString().isEmpty() ) {
            writePropThinkLinesVal("strokeColor", "#000000", "transparent", true);
        } else {
            writePropThinkLinesVal("strokeColor", "#000000", stroke.toString(), true);
        }
    } else {
        writePropThinkLinesVal("strokeColor", "#000000", "transparent", true);
    }

    if ( itm->styles().has("stroke-width") ) {
        QVariant strokeWidth = itm->styles().get("stroke-width");

        if ( strokeWidth.canConvert<CSS::MeasureUnit>() ) {
            CSS::MeasureUnit mu = strokeWidth.value<CSS::MeasureUnit>();

            if ( mu.type()==CSS::MeasureUnit::MU_PX ||  mu.type()==CSS::MeasureUnit::MU_PT) {
                writePropThinkLinesVal("strokeWidth", 1, mu.asPx());
            } else {
                qWarning()<<"Unsupported measure unit for stroke-width:"<<mu;
            }

        } else {
            qWarning()<<"Unsupported stroke-width value"<<strokeWidth.toString();
        }
    } else {
         writePropThinkLinesVal("strokeWidth", 1, 0);
    }

    if ( itm->styles().has("stroke-linejoin") ) {
        QString linejoin = itm->styles().get("stroke-linejoin").toString();
        if ( linejoin=="bevel" ) {
            writePropVal("joinStyle", "ShapePath.BevelJoin");
        } else
        if ( linejoin=="miter" ) {
            writePropVal("joinStyle", "ShapePath.MiterJoin");
        } else
        if ( linejoin=="round" ) {
            writePropVal("joinStyle", "ShapePath.RoundJoin");
        } else {
            qWarning()<<"Unsupported stroke-linejoin type:"<<linejoin;
        }
    }

    if ( itm->styles().has("stroke-linecap") ) {
        QString linecap = itm->styles().get("stroke-linecap").toString();
        if ( linecap=="butt" ) {
            writePropVal("capStyle", "ShapePath.FlatCap");
        } else
        if ( linecap=="round" ) {
            writePropVal("capStyle", "ShapePath.RoundCap");
        } else
        if ( linecap=="square" ) {
            writePropVal("capStyle", "ShapePath.SquareCap");
        } else {
            qWarning()<<"Not supported stroke-linecap type:"<<linecap;
        }
    }

    if ( itm->styles().has("stroke-miterlimit") ) {
        double miterLimit = itm->styles().get("stroke-miterlimit").value<CSS::MeasureUnit>().val();
        writePropVal("miterLimit", miterLimit);
    }

    if ( itm->styles().has("stroke-dasharray") ) {
        QList<CSS::MeasureUnit> dashList = itm->styles().get("stroke-dasharray").value<QList<CSS::MeasureUnit>>();
        if ( dashList.count()>0 ) {
            QStringList dashListStr;
            foreach (CSS::MeasureUnit mu, dashList) { dashListStr<<QString::number(mu.val()); }
            writePropVal("strokeStyle", "ShapePath.DashLine");
            writePropVal("dashPattern", QString("[%1]").arg(dashListStr.join(", ")));
        }
    }

}

/**
* @brief Make GradientStop
* @param gr
*/
void QMLGenerator::makeGradientStops(FGradient *gr)
{
    foreach(FGradient::TGradientStop gs, gr->stops()) {
        writeStartLvl("GradientStop", true);
            writePropVal("position", gs.position, false, true);
            writePropThinkLinesVal("color", "transparent", gs.color.name(QColor::HexArgb), true, true);
        writeEndLvl(true);
    }
}

/**
* @brief Создаём из CPrimitive
* @param itm
* @param visible
* @param layerEnabled
*/
void QMLGenerator::makeElement(CPrimitive *el, bool visible, bool layerEnabled)
{
    //-- Что из элементов поддерживаем пока что
    QList<int> supportedTypes = {CPrimitive::PT_PATH, CPrimitive::PT_CIRCLE, CPrimitive::PT_RECT,
                                 CPrimitive::PT_ELLIPSE, CPrimitive::PT_LINE, CPrimitive::PT_POLYLINE,
                                 CPrimitive::PT_POLYGON};

    CNodeInterfaceIterator i(el);
    while( i.next() ) {

        CPrimitive * p = i.item<CPrimitive*>();

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTLEVEL) ) {
            CPrimitive * level = i.level<CPrimitive*>();

            if ( level->type()==CPrimitive::PT_GROUP ) {                
                writeStartLvl("Item");
                    makeID(level);
                    if ( !visible ) { writePropVal("visible", false); }
                    if ( layerEnabled ) {  writePropVal("layer.enabled", true); }
                    writePropElVal("width", _settings.rootName, "width");
                    writePropElVal("height", _settings.rootName, "height");
            }                        
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTELEMENT) ) {

            if ( p->type()==CPrimitive::PT_GROUP || p->type()==CPrimitive::PT_SVG ) continue;

            if (  supportedTypes.indexOf(p->type())>-1 ) {

                QString pathCommnads = primitiveToPathCommands(p);
                if ( pathCommnads.isEmpty() ) { continue; }

                writeStartLvl("Shape");
                    makeID(p);
                    writePropElVal("width", _settings.rootName, "width");
                    writePropElVal("height", _settings.rootName, "height");

                    //-- Path
                    _qml<<tab(_lvl++)<<"ShapePath {"<<Qt::endl;
                        if ( _settings.enableScale ) { writePropElVal("scale", _settings.rootName, "scaleShape"); }

                        writeStartLvl("PathSvg");
                            writePropVal("path", pathCommnads, true);
                        writeEndLvl();

                        //-- Styles
                        makeStroke(p);
                        bool simpleFilled = makeFill(p, true);

                    writeEndLvl();

                    if ( !simpleFilled ) { makeFill(p, false); }

                    //-- Clip path
                    if ( p->styles().has("clip-path") ) {
                        CDef * def = _defs.get(p->styles().get("clip-path").toUrl());

                        if ( def==nullptr || def->defType()!=CDef::DF_CLIPPATH ) {
                            qWarning()<<"Not supported clip-path resource";
                            continue;
                        }

                        FClipPath * cp = static_cast<FClipPath*>(def);
                        writePropVal("layer.enabled", true);
                        writeStartLvlProp("layer.effect", "ShaderEffect");
                            writePropVal("fragmentShader", "qrc:/mask.frag.qsb", true);
                            makeElement(cp->clipPath, false, true);
                            writePropVal("property var maskSource", cp->clipPath->ID());
                        writeEndLvl();
                    }

                    makeOpacity(p);

                    makeTransform(p->transform());

                writeEndLvl();

                i.nextLevel();
                continue;
            } else {
                qWarning()<<"Not supported SVG element"<<p->type();
            }
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_ENDLEVEL) ) {
            CPrimitive * level = i.level<CPrimitive*>();
            if ( level->type()==CPrimitive::PT_GROUP) {
               writeEndLvl();
            }
        }

    }
}

/**
* @brief Make transformation matrix
* @param m
*/
void QMLGenerator::makeTransform(const CMatrix &m)
{
    _qml<<tab(_lvl)<<"transform: "<<"Matrix4x4 {"<<"matrix: Qt.matrix4x4("
        <<m.getAt(0,0)<<", "<<m.getAt(1,0)<<", "<<"0"<<", "<<m.getAt(2,0)<<", "
        <<m.getAt(0,1)<<", "<<m.getAt(1,1)<<", "<<"0"<<", "<<m.getAt(2,1)<<", "
        <<"0, 0, 1, 0, "
        <<"0, 0, 0, 1"
    <<"); }"<<Qt::endl;
}

/**
* @brief Выводим id шник
* @param itm
*/
void QMLGenerator::makeID(CPrimitive *itm)
{
    QString id = sanitizeID(itm->ID());
    if ( id.isEmpty() ) { return; }
    writePropVal("id", id);
}

/**
* @brief propName: elementName {
* @param propName
* @param elementName
*/
void QMLGenerator::writeStartLvlProp(QString propName, QString elementName)
{
    _qml<<tab(_lvl)<<propName<<": "<<elementName<<" {"<<Qt::endl;
    _lvl++;
}

/**
* @brief elementName {
* @param elementName
* @param isInline
*/
void QMLGenerator::writeStartLvl(QString elementName, bool isInline)
{
    _qml<<tab(_lvl)<<elementName<<" {";
    if ( !isInline ) { _qml<<Qt::endl; } else { _qml<<" "; }
    _lvl++;
}

/**
* @brief }
*/
void QMLGenerator::writeEndLvl(bool isInline)
{
    _lvl--;
    _qml<<tab((!isInline)?_lvl:0)<<"}"<<Qt::endl;
}

/**
* @brief name: val |  name: "val"
* @param name
* @param val
* @param quoted
*/
void QMLGenerator::writePropVal(QString name, QVariant val, bool quoted, bool isInline)
{
    _qml<<tab((!isInline)?_lvl:0)<<name<<": ";

    if ( quoted ) {
        _qml<<"\""<<val.toString()<<"\"";
    } else {
       _qml<<val.toString();
    }

    if ( isInline ) {
        _qml<<"; ";
    } else {
        _qml<<Qt::endl;
    }
}

/**
* @brief name: (root.thinkLines)? valThink : val | name: (root.thinkLines)? "valThink" : "val"
* @param name
* @param valThink
* @param val
* @param quoted
*/
void QMLGenerator::writePropThinkLinesVal(QString name, QVariant valThink, QVariant val, bool quoted, bool isInline)
{
    _qml<<tab((!isInline)?_lvl:0)<<name<<": ";

    if ( quoted ) {
        _qml<<"("<<_settings.rootName<<".thinkLines)? \""<<valThink.toString()<<"\" : \""<<val.toString()<<"\"";
    } else {
        _qml<<"("<<_settings.rootName<<".thinkLines)? "<<valThink.toString()<<" : "<<val.toString();
    }

    if ( !isInline ) { _qml<<Qt::endl; }
}

/**
* @brief name: elementName.val
* @param name
* @param elementName
* @param val
*/
void QMLGenerator::writePropElVal(QString name, QString elementName, QVariant val)
{
    _qml<<tab(_lvl)<<name<<": ";
    _qml<<elementName<<"."<<val.toString();
    _qml<<Qt::endl;
}




