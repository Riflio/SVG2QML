#ifndef CELLIPSE_H
#define CELLIPSE_H

#include "../Assets/cprimitive.h"
#include "cbezier.h"

class CEllipse: public CPrimitive<CEllipse>
{
public:
    CEllipse(const CPoint &centerPoint = CPoint());

    typedef QPair<double, double> TRadius;

    CPoint center() const;
    TRadius radius() const;

    void setCX(double x);
    void setCY(double y);
    void setRX(double rx);
    void setRY(double ry);

    static CBezier* drawBezierEllipseQuarter(CPoint center, QSizeF size);

    bool toPath() override;

    CBoundingBox boundingBox(bool withTransform=true) const override;

private:
    TRadius _radius;

};

#endif // CELLIPSE_H
