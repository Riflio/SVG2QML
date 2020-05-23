#ifndef CELLIPSE_H
#define CELLIPSE_H

#include "../Assets/cprimitive.h"
#include "cbezier.h"

class CEllipse: public CPrimitive
{
public:
    CEllipse();

    typedef QPair<double, double> TRadius;

    CPoint center() const;
    TRadius radius() const;

    void setCX(double x);
    void setCY(double y);
    void setRX(double rx);
    void setRY(double ry);

    static CBezier* drawBezierEllipseQuarter(CPoint center, QSizeF size);

    bool toPath() override;

private:
    TRadius _radius;
    CPoint _centerPoint;
};

#endif // CELLIPSE_H
