#include "fradialgradient.h"

FRadialGradient::FRadialGradient(): FGradient()
{

}

FRadialGradient::FRadialGradient(const FRadialGradient &other): FGradient(other)
{
    _centerPoint = other._centerPoint;
    _radius = other._radius;
}

void FRadialGradient::setCX(float x)
{
    _centerPoint.setX(x);
}

void FRadialGradient::setCY(float y)
{
    _centerPoint.setY(y);
}

void FRadialGradient::setFX(float x)
{
    _offsetPoint.setX(x);
}

void FRadialGradient::setFY(float y)
{
    _offsetPoint.setY(y);
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

void FRadialGradient::setOffsetPoint(float x, float y)
{
    _offsetPoint.setX(x);
    _offsetPoint.setY(y);
}

QPointF FRadialGradient::offsetPoint() const
{
    return _offsetPoint;
}

void FRadialGradient::setRadius(float r)
{
    _radius = r;
}

float FRadialGradient::radius() const
{
    return _radius;
}
