#include "crect.h"
#include "cpath.h"
#include "cellipse.h"
#include "cline.h"

CRect::CRect(): CPrimitive(PT_RECT, _tl)
{

}

void CRect::setX(double x)
{
    _tl.setX(x);
    _points[0] = _tl;
}

void CRect::setY(double y)
{
    _tl.setY(y);
    _points[0] = _tl;
}

void CRect::setWidth(double width)
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

/**
* @brief Преобразуем в path
* @return
*/
bool CRect::toPath()
{
    CPath * p = new CPath();
    if ( Equal::almostEqual(_radius.first, 0) && Equal::almostEqual(_radius.second,0) ) { //-- Если без скруглений, то тупо 4 линии
        CLine * t = new CLine(CPoint(_tl.x(), _tl.y()), CPoint(_tl.x()+_size.width(), _tl.y()));
        CLine * r = new CLine(CPoint(_tl.x()+_size.width(),_tl.y()), CPoint(_tl.x()+_size.width(), _tl.y()+_size.height()));
        CLine * b = new CLine(CPoint(_tl.x()+_size.width(),_tl.y()+_size.height()), CPoint(_tl.x(), _tl.y()+_size.height()));
        CLine * l = new CLine(CPoint(_tl.x(),_tl.y()+_size.height()), CPoint(_tl.x(), _tl.y()));

        CNodeInterface::addNext(p, t);
        CNodeInterface::addNext(p, r);
        CNodeInterface::addNext(p, b);
        CNodeInterface::addNext(p, l);
    } else {
        //-- Придётся делать скругления через Безье. Скругление возьмём от Элипса
        CLine * t = new CLine(CPoint(_tl.x()+_radius.first, _tl.y()), CPoint(_tl.x()+_size.width()-_radius.first, _tl.y()));
        CBezier * tr = CEllipse::drawBezierEllipseQuarter(CPoint(_tl.x()+_size.width()-_radius.first, _tl.y()+_radius.second), QSizeF(-_radius.first, _radius.second));
        CLine * r = new CLine(CPoint(_tl.x()+_size.width(), _tl.y()+_radius.second), CPoint(_tl.x()+_size.width(), _tl.y()+_size.height()-_radius.second));
        CBezier * br = CEllipse::drawBezierEllipseQuarter(CPoint(_tl.x()+_size.width()-_radius.first, _tl.y()+_size.height()-_radius.second), QSizeF(-_radius.first, -_radius.second));
        CLine * b = new CLine(CPoint(_tl.x()+_size.width()-_radius.first, _tl.y()+_size.height()), CPoint(_tl.x()+_radius.first, _tl.y()+_size.height()));
        CBezier * bl = CEllipse::drawBezierEllipseQuarter(CPoint(_tl.x()+_radius.first, _tl.y()+_size.height()-_radius.second), QSizeF(_radius.first, -_radius.second));
        CLine * l = new CLine(CPoint(_tl.x(), _tl.y()+_size.height()-_radius.second), CPoint(_tl.x(), _tl.y()+_radius.second));
        CBezier * tl = CEllipse::drawBezierEllipseQuarter(CPoint(_tl.x()+_radius.first, _tl.y()+_radius.second), QSizeF(_radius.first, _radius.second));

        tr->reverse();
        bl->reverse();

        CNodeInterface::addNext(p, t);
        CNodeInterface::addNext(p, tr);
        CNodeInterface::addNext(p, r);
        CNodeInterface::addNext(p, br);
        CNodeInterface::addNext(p, b);
        CNodeInterface::addNext(p, bl);
        CNodeInterface::addNext(p, l);
        CNodeInterface::addNext(p, tl);
    }

    CNodeInterface::addNext(this, p);
    return true;
}

CPrimitive* CRect::copy() const
{
    CPrimitive * prim = new CRect(*this);
    CNodeInterface::reset(prim);
    return prim;
}
