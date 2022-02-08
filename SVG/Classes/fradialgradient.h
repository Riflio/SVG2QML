#ifndef FRADIALGRADIENT_H
#define FRADIALGRADIENT_H

#include "SVG/Assets/fgradient.h"
#include "Algebra/cpoint.h"

class FRadialGradient: public FGradient
{
public:
    FRadialGradient();
    FRadialGradient(const FRadialGradient &other);
    FRadialGradient(const FGradient &other);

    void setCX(float x);
    void setCY(float y);
    void setFX(float x);
    void setFY(float y);

    void setCenterPoint(CPoint &cp);
    CPoint centerPoint() const;
    CPoint &centerPoint();

    void setFocalPoint(CPoint &fp);
    CPoint focalPoint() const;
    CPoint &focalPoint();

    void setRadius(float r);
    float radius() const;

    void setFocalRadius(float r);
    float focalRadius() const;

    TDefType defType() const override { return DF_RADIALGRADIENT; }

    CPrimitive* copy() const override;

private:
    CPoint _centerPoint;
    CPoint _focalPoint;
    float _radius;
    float _focalRadius;

};

#endif // FRADIALGRADIENT_H
