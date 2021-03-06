#include "fgradient.h"

FGradient::FGradient(): CDef()
{

}

FGradient::FGradient(const FGradient &other): CDef(other)
{
    _stops = other._stops;
}

void FGradient::addStop(const FGradient::TGradientStop &gs)
{
    _stops.append(gs);
}

QList<FGradient::TGradientStop> FGradient::stops() const
{
    return _stops;
}

void FGradient::setTransform(const CMatrix &transform)
{
    _transform = transform;
}

CMatrix FGradient::transform() const
{
    return _transform;
}
