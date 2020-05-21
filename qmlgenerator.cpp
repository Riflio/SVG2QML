#include "qmlgenerator.h"
#include <QDebug>

QMLGenerator::QMLGenerator(QObject *parent) : SVGGenerator(parent)
{

}

SVGGenerator::GenerateStatus QMLGenerator::generateQML(QIODevice *device, CPrimitive *rootItm, const CDefs &defs)
{
    qDebug()<<"Generate QML";

    if ( !device->isOpen() && !device->open(QIODevice::WriteOnly|QIODevice::Text) ) {
        qWarning()<<"Device not opened!";
        return GS_NOFILE;
    }

    _rootItm = rootItm;
    _defs = defs;

    int lvl = 0; //-- Для отступов
    QString rootID = "svg2qml";
    QTextStream qml(device);    

    qml<<"import QtQuick.Shapes 1.14"<<"\n";
    qml<<"import QtQuick 2.14"<<"\n";
    qml<<"\n";
    qml<<"Item {"<<"\n";
    qml<<tab(lvl+1)<<"id: "<<rootID<<"\n";
    qml<<tab(lvl+1)<<"property var scaleShape: "<<"Qt.size(2, 2)"<<"\n";
    lvl++;

    CNodeInterface * itm = rootItm->down;
    do {
        CPrimitive * p = dynamic_cast<CPrimitive*>(itm);

        if ( p==nullptr ) break;

        if ( p->type()==CPrimitive::PT_PATH ) {

            qml<<tab(lvl++)<<"Shape {"<<"\n";
                qml<<tab(lvl)<<QString("id: %1").arg(sanitizeID(p->ID()))<<"\n";
                qml<<tab(lvl)<<"anchors.fill: parent"<<"\n";

                //-- Path                
                qml<<tab(lvl++)<<"ShapePath {"<<"\n";
                    qml<<tab(lvl)<<"scale: "<<rootID<<".scaleShape"<<"\n";

                    qml<<tab(lvl)<<"PathSvg {"<<"\n";
                        qml<<tab(lvl+1)<<QString("path: \"%1\"").arg(generatePath(&itm))<<"\n";
                    qml<<tab(lvl)<<"}"<<"\n";

                    //-- Styles
                    makeFill(p, lvl, qml);
                    makeStroke(p, lvl, qml);

                qml<<tab(--lvl)<<"}"<<"\n";
            qml<<tab(--lvl)<<"}"<<"\n";
        }

        if ( p->type()==CPrimitive::PT_GROUP ) {
            qml<<tab(lvl++)<<"Item {"<<"\n";

            qml<<tab(lvl)<<QString("id: %1").arg(sanitizeID(p->ID()))<<"\n";
            qml<<tab(lvl)<<"anchors.fill: parent"<<"\n";

        }

        if ( itm->down!=nullptr ) {
            itm = itm->down;
        }  else if ( itm->next!=nullptr ) {
            itm = itm->next;
        } else if ( itm->up!=nullptr ) {
            while(true) {
                if ( itm->up==nullptr ) { itm = nullptr; break; }

                if ( (static_cast<CPrimitive*>(itm))->type()==CPrimitive::PT_GROUP) {
                    lvl--;
                    qml<<tab(lvl)<<"}"<<"\n";
                }

                itm = itm->up;
                if ( itm->next!=nullptr ) { itm = itm->next; break; }
            }
        } else {
            itm = nullptr;
        }

    } while( itm!=nullptr );


    qml<<"}"<<"\n";

    qDebug()<<"Generated!";

    return GS_OK;
}


QString QMLGenerator::tab(int c)
{
    QString s;
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
    //TODO: First letter to lower and other langs symbols
    id = id.replace('-', '_').replace(' ', '_');

    return id;
}

/**
* @brief Разбираемся с заливкой
* @param itm
* @param lvl
* @param qml
*/
void QMLGenerator::makeFill(CPrimitive *itm, int &lvl, QTextStream &qml)
{
    if ( !itm->styles().has("fill") ) {
        qml<<tab(lvl)<<"fillColor: \"transparent\""<<"\n";
        return;
    }

    QVariant fill = itm->styles().get("fill");

    if ( fill.type()==QVariant::Color ) {
        qml<<tab(lvl)<<"fillColor: \""<<fill.toString()<<"\""<<"\n";
    } else
    if ( fill.type()==QVariant::Url ) {
        if ( CDefs::isCDefLink(fill.toUrl()) ) {            
            CDef * def = _defs.get(fill.toUrl());

            if ( def==nullptr ) {
                qWarning()<<"Unable find def"<<fill.toUrl();
                return;
            }

            if ( def->defType()==CDef::DF_LINEARGRADIENT ) {
                FLinearGradient * gr = dynamic_cast<FLinearGradient*>(def);
                qml<<tab(lvl++)<<"fillGradient: "<<"LinearGradient {"<<"\n";
                    qml<<tab(lvl)<<"x1:"<<gr->startPoint().x()<<"; "<<"y1:"<<gr->startPoint().y()<<"; "<<"x2:"<<gr->endPoint().x()<<"; "<<"y2:"<<gr->endPoint().y()<<";"<<"\n";
                    makeGradientStops(gr, lvl, qml);
                qml<<tab(--lvl)<<"}"<<"\n";
            }

        } else {
            qDebug()<<"Unsupported fill value url"<<fill.toString();
        }


    } else {
        qWarning()<<"Unsupported fill type"<<fill.toString();
    }
}

/**
* @brief Разбираемся с обводкой
* @param itm
* @param lvl
* @param qml
*/
void QMLGenerator::makeStroke(CPrimitive *itm, int &lvl, QTextStream &qml)
{
    if ( !itm->styles().has("stroke") ) {
        return;
    }

    QVariant stroke = itm->styles().get("stroke");

    if ( stroke.type()==QVariant::Color ) {
        qml<<tab(lvl)<<"strokeColor: \""<<stroke.toString()<<"\""<<"\n";
    } else
    if ( stroke.toString()=="none" ) {
        qml<<tab(lvl)<<"strokeColor: \"transparent\""<<"\n";
    } else {
        qWarning()<<"Unsupported stroke color value: "<<stroke.toString();
    }

    if ( itm->styles().has("stroke-width") ) {
        QVariant strokeWidth = itm->styles().get("stroke-width");

        if ( strokeWidth.canConvert<CSS::MeasureUnit>() ) {
            CSS::MeasureUnit mu = strokeWidth.value<CSS::MeasureUnit>();

            if ( mu.type()==CSS::MeasureUnit::MU_PX ||  mu.type()==CSS::MeasureUnit::MU_PT) {
                qml<<tab(lvl)<<"strokeWidth: "<<mu.asPx()<<"\n";
            } else {
                qWarning()<<"Unsupported measure unit for stroke-width:"<<mu;
            }

        } else {
            qWarning()<<"Unsupported stroke-width value"<<strokeWidth.toString();
        }
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
        qml<<tab(lvl)<<"GradientStop { position: "<<gs.position<<"; color: \""<<gs.color.name(QColor::HexArgb)<<"\"; } "<<"\n";
    }
}



