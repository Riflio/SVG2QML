#include "cdoc.h"

CDoc::CDoc(): CPrimitive(CPrimitive::PT_GROUP)
{

}

void CDoc::setTitle(QString title)
{
    _title = title;
}

QString CDoc::title() const
{
    return _title;
}
