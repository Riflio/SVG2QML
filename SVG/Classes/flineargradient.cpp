#include "flineargradient.h"

FLinearGradient::FLinearGradient() : FGradient()
{

}

FLinearGradient::FLinearGradient(const FLinearGradient &other): FGradient(other)
{
    _startPoint = other._startPoint;
    _endPoint = other._endPoint;
}

FLinearGradient::FLinearGradient(const FGradient &other): FGradient(other)
{
    if ( other.defType()==DF_LINEARGRADIENT ) {
        const FLinearGradient otherLinear = dynamic_cast<const FLinearGradient&>(other);
        _startPoint = otherLinear._startPoint;
        _endPoint = otherLinear._endPoint;
    }
}

void FLinearGradient::setX1(float x)
{
    _startPoint.setX(x);
}

void FLinearGradient::setY1(float y)
{
    _startPoint.setY(y);
}

void FLinearGradient::setX2(float x)
{
    _endPoint.setX(x);
}

void FLinearGradient::setY2(float y)
{
    _endPoint.setY(y);
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
