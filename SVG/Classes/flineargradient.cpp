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

void FLinearGradient::setStartPoint(const CPoint &s)
{
    _startPoint = s;
}

void FLinearGradient::setEndPoint(const CPoint &e)
{
    _endPoint = e;
}

CPoint FLinearGradient::startPoint() const
{
    RELDEFVAL(_startPoint);
    return _startPoint;
}

CPoint FLinearGradient::endPoint() const
{
    RELDEFVAL(_endPoint);
    return _endPoint;
}
