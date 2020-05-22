#include "ccircle.h"

CCircle::CCircle()
    :CPrimitive(PT_CIRCLE)
{

}

CCircle::CCircle(const CCircle &other)
    :CPrimitive(other)
{
    _radius = other._radius;
}


CCircle::CCircle(CPoint center, double radius)
    :CPrimitive(PT_CIRCLE, center), _radius(radius)
{

}

void CCircle::setRadius(double radius)
{
    _radius = radius;
}

double CCircle::radius() const
{
    return _radius;
}

/**
* @brief Преобразуем в путь и добавляем к себе вниз
* @return
*/
bool CCircle::toPath()
{
    CPath * path = new CPath();
    CPoint center = _points.p1();

    CBezier * b1 = drawBezierEllipseQuarter(center, QSize(-_radius, -_radius));
    CBezier * b2 = drawBezierEllipseQuarter(center, QSize(_radius, -_radius));
    CBezier * b3 = drawBezierEllipseQuarter(center, QSize(_radius, _radius));
    CBezier * b4 = drawBezierEllipseQuarter(center, QSize(-_radius, _radius));

    b2->reverse();
    b4->reverse();

    CNodeInterface::addNext(path, b1);
    CNodeInterface::addNext(path, b2);
    CNodeInterface::addNext(path, b3);
    CNodeInterface::addNext(path, b4);
    CNodeInterface::addNext(this, path);

    path->setIsClosed(true);
    return true;
}

CBezier* CCircle::drawBezierEllipseQuarter(CPoint center, QSize size) const
{
    double c = 0.55191502449;
    CPoint p1(center.x()-size.width(), center.y());
    CPoint p2(center.x()-size.width(), center.y()-(c*size.height()));
    CPoint p3(center.x()-(c*size.width()), center.y()-size.height());
    CPoint p4(center.x(), center.y()-size.height());

    CBezier * b = new CBezier(p1, p2, p3, p4);

    return b;
}

