#ifndef CDEFS_H
#define CDEFS_H

#include <QMap>
#include <QUrl>

#include "cdef.h"

/**
* @brief Список CDef
**/
class CDefs: public QMap<QString, CDef*>
{
public:
    CDefs();
    CDef * get(const QUrl &link) const;


    static bool isCDefLink(const QUrl &link);
};

#endif // CDEFS_H
