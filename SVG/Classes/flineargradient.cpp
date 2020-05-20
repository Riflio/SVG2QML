#include "flineargradient.h"

FLinearGradient::FLinearGradient() : FGradient()
{

}

FLinearGradient::FLinearGradient(const FLinearGradient &other): FGradient(other)
{
    _startPoint = other._startPoint;
    _endPoint = other._endPoint;
}

void FLinearGradient::setStartPoint(float x, float y)
{
    _startPoint.setX(x);
    _startPoint.setY(y);
}

void FLinearGradient::setEndPoint(float x, float y)
{
    _endPoint.setX(x);
    _endPoint.setY(y);
}

QPointF FLinearGradient::startPoint() const
{
    return _startPoint;
}

QPointF FLinearGradient::endPoint() const
{
    return _endPoint;
}
