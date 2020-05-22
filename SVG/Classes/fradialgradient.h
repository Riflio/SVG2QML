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

    void setOffsetPoint(float x, float y);
    QPointF offsetPoint() const;

    void setRadius(float r);
    float radius() const;

    TDefType defType() const override { return DF_RADIALGRADIENT;  }

private:
    QPointF _centerPoint;
    QPointF _offsetPoint;
    float _radius;

};

#endif // FRADIALGRADIENT_H
