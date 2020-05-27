#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "../Assets/cprimitive.h"

class CPolyline: public CPrimitive
{
public:
    CPolyline();

    void addPoint(const CPoint &p);

    bool toPath() override;
};

#endif // CPOLYLINE_H
