#include "crect.h"

CRect::CRect()
{

}

void CRect::setX(double x)
{
    _tl.setX(x);
}

void CRect::setY(double y)
{
    _tl.setY(y);
}

void CRect::seWidth(double width)
{
    _size.setWidth(width);
}

void CRect::setHeight(double height)
{
    _size.setHeight(height);
}

void CRect::setRX(double x)
{
    _radius.first = x;
}

void CRect::setRY(double y)
{
    _radius.second = y;
}

CPoint CRect::topLeft() const
{
    return _tl;
}

QSizeF CRect::size() const
{
    return _size;
}

CRect::TRadius CRect::radius() const
{
    return _radius;
}
