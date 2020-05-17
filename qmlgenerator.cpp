#include "qmlgenerator.h"
#include <QDebug>

QMLGenerator::QMLGenerator(QObject *parent) : SVGGenerator(parent)
{

}

SVGGenerator::GenerateStatus QMLGenerator::generateQML(QIODevice *device, CPrimitive *rootItm)
{
    qDebug()<<"Generate QML";

    if ( !device->isOpen() && !device->open(QIODevice::WriteOnly|QIODevice::Text) ) {
        qWarning()<<"Device not opened!";
        return GS_NOFILE;
    }

    QTextStream qml(device);

    int lvl = 0;
    qml<<"Item {"<<"\n";
    lvl++;

    CNodeInterface * itm = rootItm->down;
    do {
        CPrimitive * p = static_cast<CPrimitive*>(itm);

        if ( p==nullptr ) break;

        qDebug()<<"--PROCESS"<<p->type()<<p->ID();
        if ( p->type()==CPrimitive::PT_PATH ) {

            qml<<tab(lvl)<<"Shape {"<<"\n";
                qml<<tab(lvl+1)<<QString("id: %1").arg(p->ID())<<"\n";
                qml<<tab(lvl+1)<<"ShapePath {"<<"\n";
                    qml<<tab(lvl+2)<<"PathSvg {"<<"\n";
                        qml<<tab(lvl+3)<<QString("path: \"%1\"").arg(generatePath(&itm))<<"\n";
                    qml<<tab(lvl+2)<<"}"<<"\n";
                qml<<tab(lvl+1)<<"}"<<"\n";
            qml<<tab(lvl)<<"}"<<"\n";
        }

        if ( p->type()==CPrimitive::PT_GROUP ) {
            qml<<tab(lvl)<<"Item {"<<"\n";
            qml<<tab(lvl+1)<<QString("id: %1").arg(p->ID())<<"\n";
            lvl++;
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



