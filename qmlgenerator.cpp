#include "qmlgenerator.h"
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
    if ( !device->isOpen() && !device->open(QIODevice::WriteOnly|QIODevice::Text) ) {
        qWarning()<<"Device not opened!";
        return GS_NOFILE;
    }

    _rootItm = rootItm;
    _defs = defs;

    int lvl = 0; //-- Для отступов
    QTextStream qml(device);    

    qml<<"import QtQuick "<<_settings.versionQtQuick<<"\n";
    qml<<"import QtQuick.Shapes "<<_settings.versionQtQuickShapes<<"\n";
    qml<<"import QtGraphicalEffects "<<_settings.versionQtGraphicalEffects<<" as GE"<<"\n"; //-- AS т.к. из QtGraphicalEffects совпадают названия градиентов
    qml<<"\n";

    qml<<tab(lvl++)<<"Item {"<<"\n";
    qml<<tab(lvl)<<"id: "<<_settings.rootName<<"\n";
    qml<<tab(lvl)<<"property var scaleShape: "<<"Qt.size(1, 1)"<<"\n";
    qml<<tab(lvl)<<"property bool thinkLines: "<<"false"<<"\n";

    makeElement(rootItm, lvl, qml);

    qml<<tab(--lvl)<<"}"<<"\n";

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

    QRegExp rx("([a-zA-Z0-9_])+");
    if ( !rx.exactMatch(id) ) return "";

    return id;
}

/**
* @brief Генерируем команды для path.d из примитивов
* @param p
* @return
*/
QString QMLGenerator::primitiveToPathCommands(CPrimitive *p)
{
    QString pathCommnads = "";
    if ( p->type()==CPrimitive::PT_CIRCLE ) {
        CCircle * circle = static_cast<CCircle*>(p);
        circle->toPath();
        circle->applyTransform();
        pathCommnads = generatePath(circle->down);
    } else
    if ( p->type()==CPrimitive::PT_PATH ) {
        CPath * path = static_cast<CPath*>(p);
        path->applyTransform();
        pathCommnads = generatePath(path);
    } else
    if ( p->type()==CPrimitive::PT_RECT ) {
        CRect * rect = static_cast<CRect*>(p);
        rect->toPath();
        rect->applyTransform();
        pathCommnads = generatePath(rect->down);
    } else
    if ( p->type()==CPrimitive::PT_LINE ) {
        CLine * line = static_cast<CLine*>(p);
        line->toPath();
        line->applyTransform();
        pathCommnads = generatePath(line->down);
    } else
    if ( p->type()==CPrimitive::PT_ELLIPSE ) {
        CEllipse * ellipse = static_cast<CEllipse*>(p);
        ellipse->toPath();
        ellipse->applyTransform();
        pathCommnads = generatePath(ellipse->down);
    } else {
        qWarning()<<"Unsupported SVG element"<<p->type();
    }

    return pathCommnads;
}

/**
* @brief Разбираемся заливкой
* @param itm
* @param lvl
* @param qml
* @param isSimple - если просо цвет или градиент без трансформации
* @return
*/
bool QMLGenerator::makeFill(CPrimitive *itm, int &lvl, QTextStream &qml, bool isSimple)
{
    if ( !itm->styles().has("fill") ) {
        qml<<tab(lvl)<<"fillColor: "<<"("<<_settings.rootName<<".thinkLines)? \"transparent\" : "<<"\"transparent\""<<"\n";
        return true;
    }

    QVariant fill = itm->styles().get("fill");

    if ( fill.type()==QVariant::Color ) {
        QColor color = fill.value<QColor>();
        if ( itm->styles().has("fill-opacity") ) { color.setAlphaF(itm->styles().get("fill-opacity").value<CSS::MeasureUnit>().val());  }
        qml<<tab(lvl)<<"fillColor: "<<"("<<_settings.rootName<<".thinkLines)? \"transparent\" : "<<"\""<<color.name(QColor::HexArgb)<<"\""<<"\n";
        return true;
    } else
    if ( fill.type()==QVariant::Url ) {
        if ( CDefs::isCDefLink(fill.toUrl()) ) {
            CDef * def = _defs.get(fill.toUrl());

            if ( def==nullptr ) {
                qWarning()<<"Unable find def"<<fill.toUrl();
                return false;
            }

            if ( def->defType()==CDef::DF_LINEARGRADIENT || def->defType()==CDef::DF_RADIALGRADIENT) {
                FGradient * gr = dynamic_cast<FGradient*>(def);
                if ( gr->transform().isDefault() ) {  //-- Если нет трансформаций, то выводим как обычно
                    makeFillGradient(itm, gr, lvl, qml, def->defType());
                    return false;
                } else { //-- Есть трансформации - придётся мучаться
                    if ( isSimple ) { //-- Если режим простая заливка, то выведем просто заливку цветом
                        qml<<tab(lvl)<<"fillColor: "<<"\""<<gr->stops().last().color.name(QColor::HexArgb)<<"\""<<"\n";
                        return false;
                    } else {
                        makeFillGradientTransform(itm, gr, lvl, qml, def->defType());
                        return true;
                    }
                }
            } else {
                qWarning()<<"Unsupported fill type"<<fill.toString();
            }
        } else {
            qWarning()<<"Unsupported fill value url"<<fill.toString();
        }
    } else {
        qWarning()<<"Unsupported fill type"<<fill.toString();
    }

    return false;
}

/**
* @brief Разбираемся с прозрачностью
* @param itm
* @param lvl
* @param qml
* @return
*/
bool QMLGenerator::makeOpacity(CPrimitive *itm, int &lvl, QTextStream &qml)
{
    if ( itm->styles().has("opacity") ) {
        double opacity = itm->styles().get("opacity").value<CSS::MeasureUnit>().val();
        qml<<tab(lvl)<<"opacity: "<<opacity<<"\n";
    }
    return true;
}

/**
* @brief Выводим градиент без трансформаций
* @param itm
* @param gr
* @param lvl
* @param qml
* @param rootID
*/
void QMLGenerator::makeFillGradient(CPrimitive *itm, FGradient *gr, int &lvl, QTextStream &qml, CDef::TDefType type)
{
    Q_UNUSED(itm);
    if ( type==CDef::DF_LINEARGRADIENT ) { makeLinearGradient(dynamic_cast<FLinearGradient*>(gr), lvl, qml); } else
    if ( type==CDef::DF_RADIALGRADIENT ) { makeRadialGradient(dynamic_cast<FRadialGradient*>(gr), lvl, qml); }
}

/**
* @brief Делаем заливку градиентом с трансформацией
* @param itm
* @param lvl
* @param qml
* @param rootID
*/
void QMLGenerator::makeFillGradientTransform(CPrimitive *itm, FGradient *gr, int &lvl, QTextStream &qml, CDef::TDefType type)
{
    //-- Внутрь основной засовываем итем, внутрь такую же фигуру чисто с заливкой градиентом, к итему ещё применяем маску, что бы градиент не вылезал за его пределы
    qml<<tab(lvl++)<<"Item {"<<"\n";
        qml<<tab(lvl)<<"width: "<<_settings.rootName<<".width"<<"\n";
        qml<<tab(lvl)<<"height: "<<_settings.rootName<<".height"<<"\n";
        qml<<tab(lvl++)<<"Shape {"<<"\n";
            qml<<tab(lvl)<<"width: "<<_settings.rootName<<".width"<<"\n";
            qml<<tab(lvl)<<"height: "<<_settings.rootName<<".height"<<"\n";

            //-- Path
            qml<<tab(lvl++)<<"ShapePath {"<<"\n";
                if ( _settings.enableScale ) qml<<tab(lvl)<<"scale: "<<_settings.rootName<<".scaleShape"<<"\n";
                QString pathCommands = primitiveToPathCommands(itm);

                qml<<tab(lvl)<<"PathSvg {"<<"\n";
                    qml<<tab(lvl+1)<<QString("path: \"%1\"").arg(pathCommands)<<"\n";
                qml<<tab(lvl)<<"}"<<"\n";

                qml<<tab(lvl)<<"strokeWidth: "<<"0"<<"\n";
                qml<<tab(lvl)<<"strokeColor: "<<"\"transparent\""<<"\n";

                //-- Сам градиент
                makeFillGradient(itm, gr, lvl, qml, type);

            qml<<tab(--lvl)<<"}"<<"\n";

            //-- Трансформации
            makeTransform(gr->transform(), lvl, qml);
        qml<<tab(--lvl)<<"}"<<"\n";

        //-- Ещё и clipPath придётся сделать, что бы фигура с внутренним градиентом не вылезала шибко далеко, но контур придётся сделать меньше на половину ширины заливки
        qml<<tab(lvl)<<"layer.enabled: "<<"true"<<"\n";
        qml<<tab(lvl++)<<"layer.effect: "<<"GE.OpacityMask {"<<"\n";
            qml<<tab(lvl++)<<"maskSource: "<<"Shape {"<<"\n";
                qml<<tab(lvl)<<"width: "<<_settings.rootName<<".width"<<"\n";
                qml<<tab(lvl)<<"height: "<<_settings.rootName<<".height"<<"\n";
                qml<<tab(lvl++)<<"ShapePath {"<<"\n";
                    if ( _settings.enableScale ) qml<<tab(lvl)<<"scale: "<<_settings.rootName<<".scaleShape"<<"\n";
                    qml<<tab(lvl)<<"PathSvg {"<<"\n";
                        qml<<tab(lvl+1)<<QString("path: \"%1\"").arg(pathCommands)<<"\n";
                    qml<<tab(lvl)<<"}"<<"\n";
                    qml<<tab(lvl)<<"fillColor: "<<"\"#000000\""<<"\n";
                    qml<<tab(lvl)<<"strokeWidth: "<<"0"<<"\n";
                    qml<<tab(lvl)<<"strokeColor: "<<"\"transparent\""<<"\n";
                qml<<tab(--lvl)<<"}"<<"\n";
            qml<<tab(--lvl)<<"}"<<"\n";
        qml<<tab(--lvl)<<"}"<<"\n";
    qml<<tab(--lvl)<<"}"<<"\n";
}

/**
* @brief Делаем заливку радиальным градиентом
* @param gr
* @param lvl
* @param qml
* @param rootID
*/
void QMLGenerator::makeRadialGradient(FRadialGradient *gr, int &lvl, QTextStream &qml)
{
    qml<<tab(lvl++)<<"fillGradient: "<<" RadialGradient {"<<"\n";
        qml<<tab(lvl)<<"centerX: "<<gr->centerPoint().x()<<"; "<<"centerY: "<<gr->centerPoint().y()<<";"<<"\n";
        qml<<tab(lvl);
        qml<<"focalX: "<<((gr->focalPoint().x()==0)? "centerX" : QString::number(gr->focalPoint().x()) )<<"; ";
        qml<<"focalY: "<<((gr->focalPoint().y()==0)? "centerY" : QString::number(gr->focalPoint().y()))<<";"<<"\n";
        qml<<tab(lvl)<<"centerRadius: "<<gr->radius()<<"\n";
        qml<<tab(lvl)<<"focalRadius: "<<gr->focalRadius()<<"\n";
        makeGradientStops(gr, lvl, qml);
        qml<<tab(--lvl)<<"}"<<"\n";
}

/**
* @brief Делаем заливку линейным градиентом
* @param gr
* @param lvl
* @param qml
* @param rootID
*/
void QMLGenerator::makeLinearGradient(FLinearGradient *gr, int &lvl, QTextStream &qml)
{
    qml<<tab(lvl++)<<"fillGradient: "<<"LinearGradient {"<<"\n";
        qml<<tab(lvl)<<"x1:"<<gr->startPoint().x()<<"; "<<"y1:"<<gr->startPoint().y()<<"\n";
        qml<<tab(lvl)<<"x2:"<<gr->endPoint().x()<<"; "<<"y2:"<<gr->endPoint().y()<<"\n";
        makeGradientStops(gr, lvl, qml);
    qml<<tab(--lvl)<<"}"<<"\n";
}

/**
* @brief Разбираемся с обводкой
* @param itm
* @param lvl
* @param qml
*/
void QMLGenerator::makeStroke(CPrimitive *itm, int &lvl, QTextStream &qml)
{
    if ( itm->styles().has("stroke") ) {
        QVariant stroke = itm->styles().get("stroke");
        if ( stroke.type()==QVariant::Color ) {
            QColor color = stroke.value<QColor>();
            if ( itm->styles().has("stroke-opacity") ) { color.setAlphaF(itm->styles().get("stroke-opacity").value<CSS::MeasureUnit>().val()); }
            qml<<tab(lvl)<<"strokeColor: "<<"("<<_settings.rootName<<".thinkLines)? \"black\" : "<<"\""<<color.name(QColor::HexArgb)<<"\""<<"\n";
        } else
        if ( stroke.toString()=="none" ) {
            qml<<tab(lvl)<<"strokeColor: "<<"("<<_settings.rootName<<".thinkLines)? \"black\"  : "<< "\"transparent\""<<"\n";
        } else {
            qWarning()<<"Unsupported stroke color value: "<<stroke.toString();
        }
    } else {
        qml<<tab(lvl)<<"strokeColor: "<<"("<<_settings.rootName<<".thinkLines)? \"black\" : "<<"\"transparent\""<<"\n";
    }

    if ( itm->styles().has("stroke-width") ) {
        QVariant strokeWidth = itm->styles().get("stroke-width");

        if ( strokeWidth.canConvert<CSS::MeasureUnit>() ) {
            CSS::MeasureUnit mu = strokeWidth.value<CSS::MeasureUnit>();

            if ( mu.type()==CSS::MeasureUnit::MU_PX ||  mu.type()==CSS::MeasureUnit::MU_PT) {
                qml<<tab(lvl)<<"strokeWidth: "<<"("<<_settings.rootName<<".thinkLines)? 1 : "<<mu.asPx()<<"\n";
            } else {
                qWarning()<<"Unsupported measure unit for stroke-width:"<<mu;
            }

        } else {
            qWarning()<<"Unsupported stroke-width value"<<strokeWidth.toString();
        }
    } else {
         qml<<tab(lvl)<<"strokeWidth: "<<"("<<_settings.rootName<<".thinkLines)? 1 : 0"<<"\n";
    }

    if ( itm->styles().has("stroke-linejoin") ) {
        QString linejoin = itm->styles().get("stroke-linejoin").toString();
        if ( linejoin=="bevel" ) {
            qml<<tab(lvl)<<"joinStyle: "<<"ShapePath.BevelJoin"<<"\n";
        } else
        if ( linejoin=="miter" ) {
            qml<<tab(lvl)<<"joinStyle: "<<"ShapePath.MiterJoin"<<"\n";
        } else
        if ( linejoin=="round" ) {
            qml<<tab(lvl)<<"joinStyle: "<<"ShapePath.RoundJoin"<<"\n";
        } else {
            qWarning()<<"Unsupported stroke-linejoin type:"<<linejoin;
        }
    }

    if ( itm->styles().has("stroke-linecap") ) {
        QString linecap = itm->styles().get("stroke-linecap").toString();
        if ( linecap=="butt" ) {
            qml<<tab(lvl)<<"capStyle: "<<"ShapePath.FlatCap"<<"\n";
        } else
        if ( linecap=="round" ) {
            qml<<tab(lvl)<<"capStyle: "<<"ShapePath.RoundCap"<<"\n";
        } else
        if ( linecap=="square" ) {
            qml<<tab(lvl)<<"capStyle: "<<"ShapePath.SquareCap"<<"\n";
        } else {
            qWarning()<<"Not supported stroke-linecap type:"<<linecap;
        }
    }

    if ( itm->styles().has("stroke-miterlimit") ) {
        double miterLimit = itm->styles().get("stroke-miterlimit").value<CSS::MeasureUnit>().val();
        qml<<tab(lvl)<<"miterLimit: "<<miterLimit<<"\n";
    }

}

/**
* @brief Выводим стопы градиента
* @param gr
* @param lvl
* @param qml
*/
void QMLGenerator::makeGradientStops(FGradient *gr, int &lvl, QTextStream &qml)
{
    foreach(FGradient::TGradientStop gs, gr->stops()) {
        qml<<tab(lvl)<<"GradientStop { position: "<<gs.position<<"; color: "<<"("<<_settings.rootName<<".thinkLines)? \"transparent\" : "<<"\""<<gs.color.name(QColor::HexArgb)<<"\"; } "<<"\n";
    }
}

/**
* @brief Создаём из CPrimitive
* @param itm
* @param lvl
* @param qml
*/
void QMLGenerator::makeElement(CPrimitive *el, int &lvl, QTextStream &qml, bool firstInline)
{
    //-- Что из элементов поддерживаем пока что
    QList<int> supportedTypes = {CPrimitive::PT_PATH, CPrimitive::PT_CIRCLE, CPrimitive::PT_RECT, CPrimitive::PT_ELLIPSE, CPrimitive::PT_LINE};

    CNodeInterfaceIterator i(el);
    while( i.next() ) {

        CPrimitive * p = i.item<CPrimitive*>();

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTLEVEL) ) {
            CPrimitive * level = i.level<CPrimitive*>();

            if ( level->type()==CPrimitive::PT_GROUP ) {
                qml<<tab((firstInline)?0:lvl)<<"Item {"<<"\n";
                lvl++;
                makeID(level, lvl, qml);
                qml<<tab(lvl)<<"width: "<<_settings.rootName<<".width"<<"\n";
                qml<<tab(lvl)<<"height: "<<_settings.rootName<<".height"<<"\n";
            }                        
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_STARTELEMENT) ) {

            if ( p->type()==CPrimitive::PT_GROUP ) continue;

            if (  supportedTypes.indexOf(p->type())>-1 ) {
                QString pathCommnads = primitiveToPathCommands(p);

                if ( pathCommnads.isEmpty() ) { continue; }

                qml<<tab(lvl)<<"Shape {"<<"\n";
                    makeID(p, ++lvl, qml);
                    qml<<tab(lvl)<<"width: "<<_settings.rootName<<".width"<<"\n";
                    qml<<tab(lvl)<<"height: "<<_settings.rootName<<".height"<<"\n";

                    //-- Path
                    qml<<tab(lvl++)<<"ShapePath {"<<"\n";
                        if ( _settings.enableScale ) qml<<tab(lvl)<<"scale: "<<_settings.rootName<<".scaleShape"<<"\n";

                        qml<<tab(lvl)<<"PathSvg {"<<"\n";
                            qml<<tab(lvl+1)<<QString("path: \"%1\"").arg(pathCommnads)<<"\n";
                        qml<<tab(lvl)<<"}"<<"\n";

                        //-- Styles
                        makeStroke(p, lvl, qml);
                        bool simpleFilled = makeFill(p, lvl, qml, true);

                    qml<<tab(--lvl)<<"}"<<"\n";

                    if ( !simpleFilled ) { makeFill(p, lvl, qml, false); }

                    //-- Clip path
                    if ( p->styles().has("clip-path") ) {
                        CDef * def = _defs.get(p->styles().get("clip-path").toUrl());

                        if ( def==nullptr || def->defType()!=CDef::DF_CLIPPATH ) {
                            qWarning()<<"Not supported clip-path resource";
                            continue;
                        }

                        FClipPath * cp = static_cast<FClipPath*>(def);

                        qml<<tab(lvl)<<"layer.enabled: "<<"true"<<"\n";
                        qml<<tab(lvl++)<<"layer.effect: "<<"GE.OpacityMask {"<<"\n";
                            qml<<tab(lvl)<<"maskSource: ";
                                makeElement(cp->clipPath, lvl, qml, true);
                        qml<<tab(--lvl)<<"}"<<"\n";
                    }

                    //-- Прозрачность
                    makeOpacity(p, lvl, qml);

                qml<<tab(--lvl)<<"}"<<"\n";

                i.nextLevel();
                continue;
            } else {
                qWarning()<<"Not supported SVG element"<<p->type();
            }
        }

        if ( (i.type()&CNodeInterfaceIterator::IT_ENDLEVEL) ) {
            CPrimitive * level = i.level<CPrimitive*>();
            if ( level->type()==CPrimitive::PT_GROUP) {
                qml<<tab(--lvl)<<"}"<<"\n";

            }
        }

    }
}

/**
* @brief Выводим матрицу трансформации
* @param m
* @param lvl
* @param qml
*/
void QMLGenerator::makeTransform(const CMatrix &m, int &lvl, QTextStream &qml)
{
    qml<<tab(lvl)<<"transform: "<<"Matrix4x4 {"<<"matrix: Qt.matrix4x4("
        <<m.getAt(0,0)<<", "<<m.getAt(0,1)<<", "<<"0"<<", "<<m.getAt(0,2)<<", "
        <<m.getAt(1,0)<<", "<<m.getAt(1,1)<<", "<<"0"<<", "<<m.getAt(1,2)<<", "
        <<"0, 0, 1, 0, "
        <<"0, 0, 0, 1"
    <<"); }"<<"\n";
}

/**
* @brief Выводим id шник
* @param itm
* @param lvl
* @param qml
*/
void QMLGenerator::makeID(CPrimitive *itm, int &lvl, QTextStream &qml)
{
    QString id = sanitizeID(itm->ID());
    if ( id.isEmpty() ) return;

    qml<<tab(lvl)<<QString("id: %1").arg(id)<<"\n";
}



