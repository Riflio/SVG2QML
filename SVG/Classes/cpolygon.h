#ifndef CPOLYGON_H
#define CPOLYGON_H

#include "SVG/Assets/cprimitive.h"
#include "Algebra/cvector2d.h"

class CPolygon : public CPrimitive
{
public:
    CPolygon();
    CPolygon(const CPoints &points);
    CPolygon(const CPoint &tl, const CPoint &br);

    void cpaint(QPainter *painter, const CBoundingBox &area);
    CPoints lianirize(double tol) const;
    double area() const;
    bool isClosed() const;
    void close();

    int pointInPolygon(const CPoint &point) const;
    bool isRectangle(double tolerance=0.001) const;
    bool intersect(const CPolygon &other) const;
    bool inside(const CPolygon &other) const;
    double projectionDistance(const CPolygon &other, const CVector2D &direction) const;
    double slideDistance(const CPolygon &other, const CVector2D &direction, bool ignoreNegative) const;
    bool isEq(const CPolygon &other) const;
    bool operator==(const CPolygon &other) const;
    CPoint massCenter() const;
};

#endif // CPOLYGON_H
