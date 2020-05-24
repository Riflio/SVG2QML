#ifndef FPRIMITIVE_H
#define FPRIMITIVE_H


#include "../Assets/cdef.h"
#include "../Assets/cprimitive.h"

/**
* @brief Примитив внутри секции defs
*/
class FPrimitive: public CDef
{
public:
    FPrimitive();

    CPrimitive * primitive;

};

#endif // FPRIMITIVE_H
