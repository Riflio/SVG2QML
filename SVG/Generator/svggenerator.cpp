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

    CNodeInterfaceIterator i(rootItm);

    while ( i.next() ) {

        if ( i.type()&CNodeInterfaceIterator::IT_STARTLEVEL ) {
            CPrimitive * lvl = i.level<CPrimitive*>();
            if ( lvl->type()==CPrimitive::PT_GROUP ) {
                _xml->writeStartElement("g");
            }
        }

        if ( i.type()&CNodeInterfaceIterator::IT_STARTELEMENT ) {
            CPrimitive * p = i.item<CPrimitive*>();

            if ( p->type()==CPrimitive::PT_PATH ) {
                CPath * path = static_cast<CPath*>(p);
                _xml->writeStartElement("path");
                _xml->writeAttribute("d", generatePath(path));
                _xml->writeAttribute("style", path->styles().toString());
                _xml->writeEndElement();
                i.nextLevel();
                continue;
            }
        }


        if ( i.type()&CNodeInterfaceIterator::IT_ENDLEVEL ) {
            CPrimitive * lvl = i.level<CPrimitive*>();
            if ( lvl->type()==CPrimitive::PT_GROUP ) {
                _xml->writeEndElement();
            }
        }

    };

    _xml->writeEndElement();
    _xml->writeEndDocument();

    return GS_OK;
}

QString SVGGenerator::generatePath(CNodeInterface *item)
{
    QStringList res;

    CPath * path = static_cast<CPath*>(item);

    if ( path==nullptr ) { throw -1; }
    if ( path->down == nullptr ) return QString();

    bool isClosed = path->isClosed();
    bool isFirst = true;

    CPrimitive * p = static_cast<CPrimitive*>(path->down);

    while (true) {
        if ( p==nullptr ) break;

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
        if ( p->type()==CPrimitive::PT_ARC ) {
            CArc * a = static_cast<CArc*>(p);
            res.append("A");
            res.append(QString::number(a->rx()));
            res.append(QString::number(a->ry()));
            res.append(QString::number(a->rotation()));
            res.append((a->largeArcFlag())? "1" : "0");
            res.append((a->sweepFlag())? "1" : "0");
            res.append(a->points().p2().toString());
        } else
        if ( p->type()==CPrimitive::PT_PATH ) { //-- Составной путь. Завершаем и начинаем новый.
            if ( isClosed ) { res.append("Z"); }
            CPath * subPath = static_cast<CPath*>(p);
            isClosed = subPath->isClosed();
            isFirst = true;
            p = static_cast<CPrimitive*>(p->down);
            continue;
        }
        else {
            qWarning()<<"Unknow path type"<<p->type();
        }

        if ( p->next==nullptr ) break;
        p = static_cast<CPrimitive*>(p->next);
    }

    if ( isClosed ) res.append("Z");

    return res.join(" ");
}

