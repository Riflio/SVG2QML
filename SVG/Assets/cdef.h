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
        DF_NONE,
        DF_LINEARGRADIENT,
        DF_RADIALGRADIENT,
        DF_CLIPPATH,
        DF_CPRIMITIVE
    };

    virtual TDefType defType() const { return DF_NONE;  }


};

#endif // CDEF_H
