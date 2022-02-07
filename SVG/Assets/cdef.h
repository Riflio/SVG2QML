#ifndef CDEF_H
#define CDEF_H

#include "cnode.h"
#include "Algebra/cempty.h"

#include "cprimitive.h"

/**
* @brief Базовый клас для шаблонных элементов
*/
class CDef : public CPrimitive
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

    bool hasRelDef() const;
    CDef * relDef() const;
    void setRelDef(CDef * relDef);

#define RELDEFVAL(var) \
    if ( hasRelDef() && (dynamic_cast<decltype(this)>(relDef())!=nullptr) && !( (dynamic_cast<decltype(this)>(relDef()))->var==CEMPTY) ) { \
        return (dynamic_cast<decltype(this)>(relDef()))->var; \
    }

#define RELDEFVALN(var) \
    if ( hasRelDef() && (dynamic_cast<decltype(this)>(relDef())!=nullptr) && !( (dynamic_cast<decltype(this)>(relDef()))->var!=dynamic_cast<decltype(this)>(relDef())->var) ) { \
        return (dynamic_cast<decltype(this)>(relDef()))->var; \
    }


private:
    CDef * _relDef;
};

#endif // CDEF_H
