#ifndef CPOLYGON_H
#define CPOLYGON_H

#include "SVG/Assets/cprimitive.h"
#include "Algebra/cvector2d.h"

class CPolygon : public CPrimitive<CPolygon>
{
public:
    CPolygon();
    CPolygon(const CPoints &points);
    CPolygon(const CPoint &tl, const CPoint &br);

    void addPoint(const CPoint &p);

    CPoints lianirize(double tol) const override;
    double area() const;
    bool isClosed() const;
    void close();

    bool toPath() override;

    int pointInPolygon(const CPoint &point) const;
    bool isRectangle(double tolerance = Equal::EPS) const;
    bool intersect(const CPolygon &other) const;
    bool inside(const CPolygon &other) const;
    double projectionDistance(const CPolygon &other, const CVector2D &direction) const;
    double slideDistance(const CPolygon &other, const CVector2D &direction, bool ignoreNegative) const;
    bool isEq(const CPolygon &other) const;
    bool operator==(const CPolygon &other) const;
    CPoint massCenter() const;
};

#endif // CPOLYGON_H
