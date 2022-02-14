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

    void setCenterPoint(const CPoint &cp);
    CPoint centerPoint() const;

    void setFocalPoint(const CPoint &fp);
    CPoint focalPoint() const;

    void setRadius(float r);
    float radius() const;

    void setFocalRadius(float r);
    float focalRadius() const;

    TDefType defType() const override { return DF_RADIALGRADIENT; }

private:
    CPoint _centerPoint;
    CPoint _focalPoint;
    float _radius;
    float _focalRadius;

};

#endif // FRADIALGRADIENT_H
