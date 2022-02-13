#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "../Assets/cprimitive.h"

class CPolyline: public CPrimitive<CPolyline>
{
public:
    CPolyline();

    void addPoint(const CPoint &p);

    bool toPath() override;
};

#endif // CPOLYLINE_H
