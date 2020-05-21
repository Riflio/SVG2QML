#ifndef CCIRCLE_H
#define CCIRCLE_H

#include "../Assets/cprimitive.h"
#include "cpath.h"

class CCircle: public CPrimitive
{
public:
    CCircle();    
    CCircle(CPoint center, double radius);
    CCircle(const CCircle &other);

    void setRadius(double radius);
    double radius() const;

    CPath * toPath() const;

private:
    double _radius;
};

#endif // CCIRCLE_H
