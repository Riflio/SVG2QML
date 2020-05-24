#include "cpath.h"
#include "cbezier.h"
#include <QDebug>

CPath::CPath():
     CPrimitive(CPrimitive::PT_PATH), _isClosed(false)
{

}

bool CPath::isClosed() const
{
    return _isClosed;
}

void CPath::setIsClosed(bool closed)
{
    _isClosed = closed;
}

/**
* @brief Изменяем направление пути на противоположное
*/
void CPath::reverse()
{
    for (CNodeInterface * ni = down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pr = static_cast<CPrimitive*>(ni);
        pr->reverse();
        CNodeInterface::removeFromLevel(pr);
        CNodeInterface::addPrev(this, pr);        
    }
}

/**
* @brief Создаём параллельный путь
* @return
*/
CPath *CPath::makeOffset(double d)
{
    CPath * offsetPath = new CPath();

    for (CNodeInterface * ni = down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pr = static_cast<CPrimitive*>(ni);

        if ( pr->type()==PT_BEZIER ) {
            qDebug()<<"MAKE BEZIER OFFSET";
            CBezier * b = static_cast<CBezier*>(pr);
            QList<CBezier*> obl = b->makeOffset(d);

            qDebug()<<"--offsets beziers"<<obl.count();

            foreach (CBezier * of, obl) {
                //--
            }


        } else {
            qWarning()<<"Not supported primitive for offset"<<pr->type();
        }


    }

    return offsetPath;
}
