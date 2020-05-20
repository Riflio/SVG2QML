#include "svggenerator.h"

#include <QDebug>

SVGGenerator::SVGGenerator(QObject *parent) : QObject(parent)
{

}

SVGGenerator::GenerateStatus SVGGenerator::generate(QIODevice *device, CPrimitive * rootItm)
{
    if ( !device->isOpen() && !device->open(QIODevice::WriteOnly|QIODevice::Text) ) {
        qWarning()<<"Device not opened!";
        return GS_NOFILE;
    }

    _xml = new QXmlStreamWriter(device);

    _xml->writeStartDocument();
    _xml->writeStartElement("svg");

    CNodeInterface * itm = rootItm->down;

    do {
        CPrimitive * p = static_cast<CPrimitive*>(itm);

        if ( p==nullptr ) break;

        if ( p->type()==CPrimitive::PT_PATH ) {
            _xml->writeStartElement("path");
            _xml->writeAttribute("d", generatePath(&itm));
            _xml->writeAttribute("style", p->styles().toString());
            _xml->writeEndElement();
        }

        if ( p->type()==CPrimitive::PT_GROUP ) {
            _xml->writeStartElement("g");
        }

        if ( itm->down!=nullptr ) {
            itm = itm->down;
        } else if ( itm->next!=nullptr ) {
            itm = itm->next;
        } else if ( itm->up!=nullptr ) {
            while(true) {
                if ( itm->up==nullptr ) { itm = nullptr; break; }

                if ( (static_cast<CPrimitive*>(itm))->type()==CPrimitive::PT_GROUP) {
                    _xml->writeEndElement();
                }

                itm = itm->up;
                if ( itm->next!=nullptr ) { itm = itm->next; break; }
            }
        } else {
            itm = nullptr;
        }

    } while( itm!=nullptr );

    _xml->writeEndElement();
    _xml->writeEndDocument();

    return GS_OK;
}

QString SVGGenerator::generatePath(CNodeInterface **itm)
{
    QStringList res;

    if ( (itm==nullptr) || (*itm==nullptr) ) throw -1;
    CPath * path = static_cast<CPath*>(*itm);

    if ( path->down == nullptr) return QString();

    bool isClosed = path->isClosed();
    bool isFirst = true;

    *itm = (*itm)->down;

    while (true) {

        CPrimitive * p = static_cast<CPrimitive*>(*itm);

        if ( isFirst ) {
            res.append("M");
            res.append(p->points().p1().toString());
            isFirst = false;
        }

        if ( p->type()==CPrimitive::PT_BEZIER ) {
            CBezier * b = static_cast<CBezier*>(p);
            res.append("C");
            res.append(b->points().p2().toString());
            res.append(b->points().p3().toString());
            res.append(b->points().p4().toString());
        } else
        if ( p->type()==CPrimitive::PT_LINE ) {
            CLine * l = static_cast<CLine*>(p);
            res.append("L");
            res.append(l->points().p2().toString());
        } else
        if ( p->type()==CPrimitive::PT_PATH ) { //-- Составной путь. Завершаем и начинаем новый
            if ( isClosed ) { res.append("Z"); }
            isClosed = (static_cast<CPath*>(*itm))->isClosed();
            isFirst = true;
            *itm = (*itm)->down;
        }
        else {
            qWarning()<<"Unknow path type"<<p->type();
        }


        if ( (*itm)->next==nullptr ) break;
        *itm = (*itm)->next;
    }

    if ( isClosed ) res.append("Z");

    return res.join(" ");
}

