#include "cellipse.h"
#include "cpath.h"
#include "cbezier.h"

CEllipse::CEllipse(): CPrimitive(PT_ELLIPSE, _centerPoint)
{

}

CPoint CEllipse::center() const
{
    return _centerPoint;
}

CEllipse::TRadius CEllipse::radius() const
{
    return _radius;
}

void CEllipse::setCX(double x)
{
    _centerPoint.setX(x);
    _points[0] = _centerPoint;
}

void CEllipse::setCY(double y)
{
    _centerPoint.setY(y);
    _points[0] = _centerPoint;
}

void CEllipse::setRX(double rx)
{
    _radius.first = rx;
}

void CEllipse::setRY(double ry)
{
    _radius.second = ry;
}

/**
* @brief Выводим часть элипса
* @param center
* @param size
* @return
*/
CBezier *CEllipse::drawBezierEllipseQuarter(CPoint center, QSizeF size)
{
    double c = 0.55191502449;
    CPoint p1(center.x()-size.width(), center.y());
    CPoint p2(center.x()-size.width(), center.y()-(c*size.height()));
    CPoint p3(center.x()-(c*size.width()), center.y()-size.height());
    CPoint p4(center.x(), center.y()-size.height());

    CBezier * b = new CBezier(p1, p2, p3, p4);

    return b;
}

bool CEllipse::toPath()
{
    CPath * path = new CPath();

    CBezier * b1 = drawBezierEllipseQuarter(_centerPoint, QSizeF(_radius.first, _radius.second));
    CBezier * b2 = drawBezierEllipseQuarter(_centerPoint, QSizeF(-_radius.first, _radius.second));
    CBezier * b3 = drawBezierEllipseQuarter(_centerPoint, QSizeF(-_radius.first, -_radius.second));
    CBezier * b4 = drawBezierEllipseQuarter(_centerPoint, QSizeF(_radius.first, -_radius.second));

    b2->reverse();
    b4->reverse();

    CNodeInterface::addNext(path, b1);
    CNodeInterface::addNext(path, b2);
    CNodeInterface::addNext(path, b3);
    CNodeInterface::addNext(path, b4);

    CNodeInterface::addNext(this, path);
    return true;
}

CPrimitive* CEllipse::copy() const
{
    CPrimitive * prim = new CEllipse(*this);
    CNodeInterface::reset(prim);
    return prim;
}


















