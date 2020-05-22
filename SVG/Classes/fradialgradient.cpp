#include "fradialgradient.h"

FRadialGradient::FRadialGradient(): FGradient(), _radius(0), _focalRadius(0)
{

}

FRadialGradient::FRadialGradient(const FRadialGradient &other): FGradient(other)
{
    _centerPoint = other._centerPoint;
    _focalPoint = other._focalPoint;
    _radius = other._radius;
    _focalRadius = other._focalRadius;
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
    _focalPoint.setX(x);
}

void FRadialGradient::setFY(float y)
{
    _focalPoint.setY(y);
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

void FRadialGradient::setFocalPoint(float x, float y)
{
    _focalPoint.setX(x);
    _focalPoint.setY(y);
}

QPointF FRadialGradient::focalPoint() const
{
    return _focalPoint;
}

void FRadialGradient::setRadius(float r)
{
    _radius = r;
}

float FRadialGradient::radius() const
{
    return _radius;
}

void FRadialGradient::setFocalRadius(float r)
{
    _focalRadius = r;
}

float FRadialGradient::focalRadius() const
{
    return _focalRadius;
}
