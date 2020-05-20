#ifndef FRADIALGRADIENT_H
#define FRADIALGRADIENT_H

#include <QPointF>
#include "../Assets/fgradient.h"

class FRadialGradient: public FGradient
{
public:
    FRadialGradient();
    FRadialGradient(const FRadialGradient &other);

    void setCenterPoint(float x, float y);
    QPointF centerPoint() const;

    void setRadius(float r);
    float radius() const;

private:
    QPointF _centerPoint;
    float _radius;

};

#endif // FRADIALGRADIENT_H
