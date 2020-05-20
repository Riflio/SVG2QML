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

    CPrimitive * target;


};

#endif // FCLIPPATH_H
