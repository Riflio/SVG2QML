#include "csize.h"
#include "math.h"

CSize::CSize(): _width(NAN), _height(NAN)
{

}

CSize::CSize(double w, double h): _width(w), _height(h)
{    
}

CSize::CSize(const CPoint& tl, const CPoint& br): _width(br.x()-tl.x()), _height(br.y()-tl.y())
{
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

bool CSize::isZero(double tolerance) const
{
    return ( Equal::almostEqual(_width, 0.0, tolerance) && Equal::almostEqual(_height, 0.0, tolerance) );
}

bool CSize::operator==(const CEmptyPriv&) const
{
    return (_width!=_width || _height!=_height);
}
