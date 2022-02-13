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
    for (INodeInterface * ni = down(); ni!=nullptr; ni=ni->next()) {
        IPrimitive * pr = dynamic_cast<IPrimitive*>(ni);
        pr->reverse();        
        pr->removeFromLevel();
        addPrev(pr);
    }
}

/**
* @brief Создаём параллельный путь
* @param d - смещение
* @return
*/
CPath *CPath::makeOffset(double d)
{
    CPath * offsetPath = new CPath();
    offsetPath->setIsClosed(isClosed());

    //TODO: Доделать смещение остальных примитивов
    for (INodeInterface * ni = down(); ni!=nullptr; ni=ni->next()) {
        IPrimitive * pr = dynamic_cast<IPrimitive*>(ni);

        if ( pr->type()==PT_BEZIER ) {            
            CBezier * b = dynamic_cast<CBezier*>(pr);
            QList<CBezier*> obl = b->makeOffset(d);
            foreach (CBezier * ob, obl) {
                offsetPath->addNext(ob);
            }
        } else {
            qWarning()<<"Not supported primitive for offset"<<pr->type();
        }

    }

    return offsetPath;
}
