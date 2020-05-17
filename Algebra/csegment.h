#ifndef CSEGMENT_H
#define CSEGMENT_H


#include "Algebra/cpoint.h"
#include "cvector2d.h"
#include "equal.h"

class CSegment
{
public:
    CSegment(const CPoint &a, const CPoint &b);

    bool onSegment(const CPoint &p) const;
    double cross(const CPoint &p) const;
    double dot(const CPoint &p) const;
    double len2() const;
    CPoint intersect(const CSegment &other, bool infinite) const;
    double pointDistance(const CPoint &p, const CVector2D &normal, bool infinite) const;
    double segmentDistance(const CSegment &other, const CVector2D &direction) const;

private:
    CPoint _a;
    CPoint _b;
};

#endif // CSEGMENT_H
