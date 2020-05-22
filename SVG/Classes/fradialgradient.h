#ifndef FRADIALGRADIENT_H
#define FRADIALGRADIENT_H

#include <QPointF>
#include "../Assets/fgradient.h"

class FRadialGradient: public FGradient
{
public:
    FRadialGradient();
    FRadialGradient(const FRadialGradient &other);

    void setCX(float x);
    void setCY(float y);
    void setFX(float x);
    void setFY(float y);

    void setCenterPoint(float x, float y);
    QPointF centerPoint() const;

    void setFocalPoint(float x, float y);
    QPointF focalPoint() const;

    void setRadius(float r);
    float radius() const;

    void setFocalRadius(float r);
    float focalRadius() const;

    TDefType defType() const override { return DF_RADIALGRADIENT;  }

private:
    QPointF _centerPoint;
    QPointF _focalPoint;
    float _radius;
    float _focalRadius;

};

#endif // FRADIALGRADIENT_H
