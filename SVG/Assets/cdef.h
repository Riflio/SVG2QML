#ifndef CDEF_H
#define CDEF_H

#include "cnode.h"

/**
* @brief Базовый клас для шаблонных элементов
*/
class CDef : public CNodeInterface
{
public:
    explicit CDef();

    enum TDefType {
        DF_LINEARGRADIENT,
        DF_CLIPPATH,
        DF_CPRIMITIVE
    };

};

#endif // CDEF_H
