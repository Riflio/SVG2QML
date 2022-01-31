#include "csize.h"
#include "math.h"

CSize::CSize(): _width(0), _height(0), _epsilon(Equal::EPS)
{

}

CSize::CSize(double w, double h)
{
    _width = w;
    _height = h;
    _epsilon = Equal::EPS;
}

double CSize::width() const
{
    return _width;
}

double CSize::height() const
{
    return _height;
}

void CSize::setSize(double w, double h)
{
    _width = w;
    _height = h;
}

void CSize::setWidth(double w)
{
    _width = w;
}

void CSize::setHeight(double h)
{
    _height = h;
}

bool CSize::isZero() const
{
    return ( (fabs(_width)<_epsilon) && (fabs(_height)<_epsilon) );
}
