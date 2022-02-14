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

SVGGenerator::GenerateStatus QMLGenerator::generateQML(QIODevice *device, IPrimitive *rootItm, const CDefs &defs)
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
QString QMLGenerator::primitiveToPathCommands(IPrimitive *p, double offset, CMatrix transforms)
{
    CPath * tPath = nullptr;

    if ( p->type()==IPrimitive::PT_CIRCLE ) {
        CCircle * circle = dynamic_cast<CCircle*>(p);
        circle->toPath();
        tPath = dynamic_cast<CPath*>(circle->down());
    } else
    if ( p->type()==IPrimitive::PT_PATH ) {
        CPath * path = dynamic_cast<CPath*>(p);
        tPath = path;
    } else
    if ( p->type()==IPrimitive::PT_RECT ) {
        CRect * rect = dynamic_cast<CRect*>(p);
        rect->toPath();
        tPath = dynamic_cast<CPath*>(rect->down());
    } else
    if ( p->type()==IPrimitive::PT_POLYLINE ) {
        CPolyline * polyline = dynamic_cast<CPolyline*>(p);
        polyline->toPath();
        tPath = dynamic_cast<CPath*>(polyline->down());
    } else
    if ( p->type()==IPrimitive::PT_POLYGON ) {
        CPolygon * polygon = dynamic_cast<CPolygon*>(p);
        polygon->toPath();
        tPath = dynamic_cast<CPath*>(polygon->down());
    } else
    if ( p->type()==IPrimitive::PT_LINE ) {
        CLine * line = dynamic_cast<CLine*>(p);
        line->toPath();
        tPath = dynamic_cast<CPath*>(line->down());
    } else
    if ( p->type()==IPrimitive::PT_ELLIPSE ) {
        CEllipse * ellipse = dynamic_cast<CEllipse*>(p);
        ellipse->toPath();
        tPath = dynamic_cast<CPath*>(ellipse->down());
    } else {
        qWarning()<<"Unsupported SVG element"<<p->type();
        return "";
    }

    if ( offset>0 ) { tPath = tPath->makeOffset(offset); }

    IPrimitive * tPathClone = tPath->clone();

    tPathClone->applyTransform(transforms);

    QString pathCommnads = generatePath(tPathClone);

    return pathCommnads;
}

/**
* @brief Разбираемся заливкой
* @param itm
* @param isSimple - если просо цвет или градиент без трансформации
* @return
*/
bool QMLGenerator::makeFill(QShape *shape, bool isSimple)
{
    if ( !shape->itm->styles().has("fill") ) {
        writePropThinkLinesVal("fillColor", "transparent", "#000000", true);
        return true;
    }

    QVariant fill = shape->itm->styles().get("fill");

    if ( fill.typeId()==QMetaType::QColor ) {
        QColor color = fill.value<QColor>();
        if ( shape->itm->styles().has("fill-opacity") ) { color.setAlphaF(shape->itm->styles().get("fill-opacity").value<CSS::MeasureUnit>().val());  }
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
                    makeFillGradient(shape, gr, def->defType());
                    return true;
                } else { //-- Есть трансформации - придётся мучаться
                    if ( isSimple ) { //-- Если режим простая заливка, то выведем просто заливку цветом
                        writePropVal("fillColor", gr->stops().last().color.name(QColor::HexArgb), true);
                        return false;
                    } else {
                        makeFillGradientTransform(shape, gr, def->defType());
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
* @param shape
* @return
*/
bool QMLGenerator::makeOpacity(QShape *shape)
{
    if ( shape->itm->styles().has("opacity") ) {
        double opacity = shape->itm->styles().get("opacity").value<CSS::MeasureUnit>().val();
        writePropVal("opacity", opacity);
    }
    return true;
}

/**
* @brief Выводим градиент без трансформаций
* @param shape
* @param gr
* @param type
*/
void QMLGenerator::makeFillGradient(QShape *shape, FGradient *gr, CDef::TDefType type)
{
    if ( type==CDef::DF_LINEARGRADIENT ) {
        FLinearGradient * lgr = dynamic_cast<FLinearGradient*>(gr);
        lgr->setStartPoint(lgr->startPoint() - shape->boundingBox.tl());
        lgr->setEndPoint(lgr->endPoint() - shape->boundingBox.tl());
        makeLinearGradient(lgr);
    } else
    if ( type==CDef::DF_RADIALGRADIENT ) {
        FRadialGradient * rgr = dynamic_cast<FRadialGradient*>(gr);
        rgr->setCenterPoint(rgr->centerPoint() - shape->boundingBox.tl());
        makeRadialGradient(rgr);
    }
}

/**
* @brief Makefill if has gradientTransform
* @param itm
* @param gr
* @param type
*/
void QMLGenerator::makeFillGradientTransform(QShape *shape, FGradient *gr, CDef::TDefType type)
{
    //-- Внутрь основной засовываем итем, внутрь фигуру (PathLine - rect для line gradient или PathAngleArc - circle) с заливкой градиентом, к итему ещё применяем маску, что бы градиент не вылезал за его пределы

    writeStartLvl("Item");
        writePropVal("width", "parent.width");
        writePropVal("height", "parent.height");

        writeStartLvl("Shape");
            QString shapeGrId = QString("%1_gr").arg(sanitizeID(shape->itm->ID()));
            writePropVal("id", shapeGrId, true);
            writePropVal("width", "parent.width");
            writePropVal("height", "parent.height");

            //-- Path
            writeStartLvl("ShapePath");                

                if ( type==CDef::DF_LINEARGRADIENT ) {
                    //-- Делаем строго горизонтальным с началом x1y1, а поворот между x1y1 и x2y2 добавляем к матрице трансформации что бы фигура была по оси x1y1 x2y2
                    FLinearGradient * lgr = dynamic_cast<FLinearGradient*>(gr);

                    double l = lgr->startPoint().lengthTo(lgr->endPoint()); //-- Gradient length
                    double w = 100; //-- Gradient continuation before first and after last stop points
                    double h = 300; //TODO: As max(width, height) of bouding box?

                    CPoint sp = lgr->startPoint();
                    CPoint ep = lgr->endPoint();

                    writePropVal("startX", sp.x()-w);
                    writePropVal("startY", sp.y()-h);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", sp.x()+l+w, false, true);
                        writePropVal("y", sp.y()-h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", sp.x()+l+w, false, true);
                        writePropVal("y", sp.y()+h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", sp.x()-w, false, true);
                        writePropVal("y", sp.y()+h, false, true);
                    writeEndLvl(true);

                    writeStartLvl("PathLine", true);
                        writePropVal("x", sp.x()-w, false, true);
                        writePropVal("y", sp.y()-h, false, true);
                    writeEndLvl(true);

                    double rotation = sp.angle(sp-CPoint(0.0, 1.0), ep);

                    CMatrix transformMatrix = CMatrix::identity(3, 3);
                    transformMatrix.multiplication(gr->transform());

                    CPoint originPoint = sp;
                    originPoint.transform(transformMatrix);

                    CMatrix rotM = CMatrix::identity(3, 3);
                    rotM.translate(originPoint.x(), originPoint.y());
                    rotM.rotate(rotation-M_PI_2);
                    rotM.translate(-originPoint.x(), -originPoint.y());

                    rotM.multiplication(transformMatrix);

                    gr->setTransform(rotM);

                    lgr->setStartPoint(sp);
                    lgr->setEndPoint(sp+CPoint(l, 0));

                    makeLinearGradient(lgr);

                } else
                if ( type==CDef::DF_RADIALGRADIENT ) {
                    FRadialGradient * rgr = dynamic_cast<FRadialGradient*>(gr);
                    //-- Делаем обычную окружность с градиентом, потом трансформациями доводим до кондиции

                    double rr = 100; //-- Gradient continuation

                    writeStartLvl("PathAngleArc");
                        writePropVal("centerX", rgr->centerPoint().x());
                        writePropVal("centerY", rgr->centerPoint().y());
                        writePropVal("radiusX", rgr->radius()+rr);
                        writePropVal("radiusY", rgr->radius()+rr);
                        writePropVal("startAngle", 0);
                        writePropVal("sweepAngle", 360);
                    writeEndLvl();

                    makeRadialGradient(rgr);
                }

                writePropVal("strokeWidth", 0);
                writePropVal("strokeColor", "transparent", true);

            writeEndLvl();

            //-- Учитываем положение самой фигуры
            CMatrix mItm = CMatrix::identity(3, 3);
            mItm.translate(-shape->boundingBox.left(), -shape->boundingBox.top());
            mItm.multiplication(gr->transform());
            gr->setTransform(mItm);

            makeTransform(gr->transform());

        writeEndLvl();

        //-- Ещё и clipPath придётся сделать, что бы фигура с внутренним градиентом не вылезала шибко далеко, но контур придётся сделать меньше на половину ширины заливки        
        writePropVal("layer.enabled", true);
        writeStartLvlProp("layer.effect", "ShaderEffect");
            writeStartLvl("Shape");
                QString shapeGrClipId = QString("%1_grcl").arg(sanitizeID(shape->itm->ID()));
                writePropVal("id", shapeGrClipId);
                writePropVal("visible", false);
                writePropVal("layer.enabled", true);
                writePropVal("width", "parent.width");
                writePropVal("height", "parent.height");
                writeStartLvl("ShapePath");
                    writeStartLvl("PathSvg");
                        writePropVal("path", shape->pathCommands, true);
                    writeEndLvl();
                    writePropVal("fillColor", "#000000", true);
                    writePropVal("strokeWidth", 0);
                    writePropVal("strokeColor", "transparent", true);
                writeEndLvl();
            writeEndLvl();
            writePropVal("fragmentShader", "qrc:/Shaders/mask.frag.qsb", true); //./qsb --glsl 100es,120,150 --hlsl 50 --msl 12  -o mask.frag.qsb mask.frag
            writePropVal("property var maskSource", shapeGrClipId);
        writeEndLvl();


    writeEndLvl();
}

/**
* @brief Делаем заливку радиальным градиентом
* @param gr
*/
void QMLGenerator::makeRadialGradient(FRadialGradient * gr)
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
*/
void QMLGenerator::makeLinearGradient(FLinearGradient * gr)
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
void QMLGenerator::makeStroke(QShape *shape)
{
    if ( shape->itm->styles().has("stroke") ) {
        QVariant stroke = shape->itm->styles().get("stroke");
        if ( stroke.typeId()==QMetaType::QColor ) {
            QColor color = stroke.value<QColor>();
            if ( shape->itm->styles().has("stroke-opacity") ) { color.setAlphaF(shape->itm->styles().get("stroke-opacity").value<CSS::MeasureUnit>().val()); }
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

    if ( shape->itm->styles().has("stroke-width") ) {
        QVariant strokeWidth = shape->itm->styles().get("stroke-width");

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

    if ( shape->itm->styles().has("stroke-linejoin") ) {
        QString linejoin = shape->itm->styles().get("stroke-linejoin").toString();
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

    if ( shape->itm->styles().has("stroke-linecap") ) {
        QString linecap = shape->itm->styles().get("stroke-linecap").toString();
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

    if ( shape->itm->styles().has("stroke-miterlimit") ) {
        double miterLimit = shape->itm->styles().get("stroke-miterlimit").value<CSS::MeasureUnit>().val();
        writePropVal("miterLimit", miterLimit);
    }

    if ( shape->itm->styles().has("stroke-dasharray") ) {
        QList<CSS::MeasureUnit> dashList = shape->itm->styles().get("stroke-dasharray").value<QList<CSS::MeasureUnit>>();
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
* @param el
*/
void QMLGenerator::makeElement(IPrimitive *el)
{
    //-- Что из элементов поддерживаем пока что
    QList<int> supportedTypes = {IPrimitive::PT_PATH, IPrimitive::PT_CIRCLE, IPrimitive::PT_RECT,
                                 IPrimitive::PT_ELLIPSE, IPrimitive::PT_LINE, IPrimitive::PT_POLYLINE,
                                 IPrimitive::PT_POLYGON};

    CNodeInterfaceIterator i(el);
    while( i.next() ) {

        IPrimitive * p = i.item<IPrimitive*>();

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTLEVEL) ) {
            IPrimitive * level = i.level<IPrimitive*>();

            if ( level->type()==IPrimitive::PT_GROUP  ) {
                writeStartLvl("Item");
                    makeID(level);

                    writePropVal("width", "parent.width");
                    writePropVal("height", "parent.height");

                    writePropVal("transformOrigin", "Item.TopLeft");

                    makeTransform(level->transformation());
            }
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTELEMENT) ) {

            if ( p->type()==IPrimitive::PT_GROUP || p->type()==IPrimitive::PT_SVG ) continue;

            if ( supportedTypes.indexOf(p->type())>-1 ) {

                //-- Сделаем, что бы фигура имел координаты и размер в соответствии с содержимым
                CMatrix tr = p->transformation();

                //-- Смещаем содержимое в 0
                CBoundingBox bb = p->boundingBox(false);
                CMatrix m = CMatrix::identity(3, 3);
                m.translate(-bb.left(), -bb.top());
                p->applyTransform(m);

                CPoint tl(bb.tl());
                tl.transform(tr);

                //-- Из матрицы трансформации уберём вращение со смещением и назначим самой фигуре
                double A = atan2(tr.getAt(0, 1), tr.getAt(1, 1));
                double TX = tr.getAt(2, 0);
                double TY = tr.getAt(2, 1);
                CMatrix r = CMatrix::identity(3, 3);
                r.rotate(-A);
                r.translate(-TX, -TY);
                p->setTransformation(tr.multiplication(r));

                QShape * shape = new QShape(p); //TODO: Delete
                shape->pathCommands = primitiveToPathCommands(p, 0);;
                shape->x = tl.x();
                shape->y = tl.y();
                shape->r = A;
                shape->boundingBox = bb;

                if ( shape->pathCommands.isEmpty() ) { continue; }

                writeStartLvl("Shape");
                    makeID(p);

                    writePropVal("x", shape->x);
                    writePropVal("y", shape->y);
                    writePropVal("rotation", shape->r*180.0/M_PI);
                    writePropVal("width", bb.width());
                    writePropVal("height", bb.height());
                    writePropVal("clip", false);
                    writePropVal("transformOrigin", "Item.TopLeft");

                    //-- Path
                    _qml<<tab(_lvl++)<<"ShapePath {"<<Qt::endl;                        

                        writeStartLvl("PathSvg");
                            writePropVal("path", shape->pathCommands, true);
                        writeEndLvl();

                        //-- Styles
                        makeStroke(shape);
                        bool simpleFilled = makeFill(shape, true);

                    writeEndLvl();

                    if ( !simpleFilled ) { makeFill(shape, false); }

                    //-- Clip path
                    if ( p->styles().has("clip-path") ) {
                        CDef * def = _defs.get(p->styles().get("clip-path").toUrl());

                        if ( def==nullptr || def->defType()!=CDef::DF_CLIPPATH ) { qWarning()<<"Not supported clip-path resource"; continue; }

                        FClipPath * cp = static_cast<FClipPath*>(def);
                        writePropVal("layer.enabled", true);
                        writeStartLvlProp("layer.effect", "ShaderEffect");
                            writeStartLvl("Item");
                                QString clipItemWrapperId = sanitizeID(QString("%1_clip_wrapper").arg(cp->clipPath->ID()));
                                writePropVal("id", clipItemWrapperId);
                                writePropVal("width", "parent.width");
                                writePropVal("height", "parent.height");
                                writePropVal("visible", "false");
                                writePropVal("layer.enabled", "true");

                                cp->clipPath->setTransformation(cp->clipPath->transformation().translate(-shape->x, -shape->y));

                                makeElement(cp->clipPath);
                            writeEndLvl();

                            writePropVal("fragmentShader", "qrc:/Shaders/mask.frag.qsb", true);
                            writePropVal("property var maskSource", clipItemWrapperId);
                        writeEndLvl();
                    }

                    makeOpacity(shape);
                    makeTransform(p->transformation());
                writeEndLvl();

                i.nextLevel();
                continue;
            } else {
                qWarning()<<"Not supported SVG element"<<p->type();
            }
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_ENDLEVEL) ) {
            IPrimitive * level = i.level<IPrimitive*>();
            if ( level->type()==IPrimitive::PT_GROUP) {
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
void QMLGenerator::makeID(IPrimitive *itm)
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
