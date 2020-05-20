#include "fradialgradient.h"

FRadialGradient::FRadialGradient(): FGradient()
{

}

FRadialGradient::FRadialGradient(const FRadialGradient &other): FGradient(other)
{
    _centerPoint = other._centerPoint;
    _radius = other._radius;
}

void FRadialGradient::setCenterPoint(float x, float y)
{
    _centerPoint.setX(x);
    _centerPoint.setY(y);
}

QPointF FRadialGradient::centerPoint() const
{
    return _centerPoint;
}

void FRadialGradient::setRadius(float r)
{
    _radius = r;
}

float FRadialGradient::radius() const
{
    return _radius;
}
