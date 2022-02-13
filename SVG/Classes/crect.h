#ifndef CRECT_H
#define CRECT_H

#include "../Assets/cprimitive.h"
#include "Algebra/csize.h"

class CRect: public CPrimitive<CRect>
{
public:
    CRect(const CPoint &tl = CPoint(), const CPoint &br = CPoint());
    typedef QPair<double, double> TRadius;

    void setX(double x);
    void setY(double y);
    void setWidth(double width);
    void setHeight(double height);
    void setRX(double x);
    void setRY(double y);

    CPoint topLeft() const;
    CPoint bottomRight() const;
    CSize size() const;

    TRadius radius() const;

    bool toPath() override;

private:
    TRadius _radius;

};

#endif // CRECT_H
