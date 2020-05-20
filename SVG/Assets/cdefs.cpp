#include "cdefs.h"

CDefs::CDefs()
{

}

CDef *CDefs::get(const QUrl &link) const
{
    QString id = link.toString().mid(1, -1);
    return value(id, nullptr);
}

bool CDefs::isCDefLink(const QUrl &link)
{
 return (link.toString().left(1)=="#");
}
