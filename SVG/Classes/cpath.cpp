#include "cpath.h"
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
CPath *CPath::makeOffset()
{

}
