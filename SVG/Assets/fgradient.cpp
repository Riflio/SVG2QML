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
    QList<FGradient::TGradientStop> stopsList;

    if ( hasRelDef() ) {
        FGradient * rg = dynamic_cast<FGradient *>(relDef());
        stopsList.append(rg->stops());
    }

    stopsList.append(_stops); //TODO: Combine by offset?

    return stopsList;
}

void FGradient::setTransform(const CMatrix &transform)
{
    _transform = transform;
}

CMatrix FGradient::transform() const
{
    return _transform;
}
