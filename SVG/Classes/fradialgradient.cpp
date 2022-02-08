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

FRadialGradient::FRadialGradient(const FGradient &other): FGradient(other), _radius(0), _focalRadius(0)
{
    if ( other.defType()==DF_RADIALGRADIENT ) {
        const FRadialGradient otherRadial = dynamic_cast<const FRadialGradient&>(other);
        _centerPoint = otherRadial._centerPoint;
        _focalPoint = otherRadial._focalPoint;
        _radius = otherRadial._radius;
        _focalRadius = otherRadial._focalRadius;
    }
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

void FRadialGradient::setCenterPoint(CPoint& cp)
{
    _centerPoint = cp;
}

CPoint FRadialGradient::centerPoint() const
{
    RELDEFVAL(_centerPoint);
    return _centerPoint;
}

CPoint& FRadialGradient::centerPoint()
{
    RELDEFVAL(_centerPoint);
    return _centerPoint;
}

void FRadialGradient::setFocalPoint(CPoint& fp)
{
    _focalPoint = fp;
}

CPoint FRadialGradient::focalPoint() const
{
    RELDEFVAL(_focalPoint);
    return _focalPoint;
}

CPoint& FRadialGradient::focalPoint()
{
    RELDEFVAL(_focalPoint);
    return _focalPoint;
}

void FRadialGradient::setRadius(float r)
{
    _radius = r;
}

float FRadialGradient::radius() const
{
    RELDEFVALN(_radius);
    return _radius;
}

void FRadialGradient::setFocalRadius(float r)
{
    _focalRadius = r;
}

float FRadialGradient::focalRadius() const
{
    RELDEFVALN(_focalRadius);
    return _focalRadius;
}

CPrimitive* FRadialGradient::copy() const
{
    CPrimitive * prim = new FRadialGradient(*this);
    CNodeInterface::reset(prim);
    return prim;
}
