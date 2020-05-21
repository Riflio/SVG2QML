#ifndef FCLIPPATH_H
#define FCLIPPATH_H

#include "../Assets/cdef.h"
#include "../Assets/cprimitive.h"

/**
* @brief Контур обрезки
*/
class FClipPath : public CDef
{    
public:
    explicit FClipPath();

    CPrimitive * clipPath;

    virtual TDefType defType() const { return DF_CLIPPATH;  }

};

#endif // FCLIPPATH_H
