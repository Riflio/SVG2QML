#include "cellipse.h"
#include "cpath.h"
#include "cbezier.h"

CEllipse::CEllipse(const CPoint& centerPoint): CPrimitive(PT_ELLIPSE, centerPoint)
{

}

CPoint CEllipse::center() const
{
    return _points[0];
}

CEllipse::TRadius CEllipse::radius() const
{
    return _radius;
}

void CEllipse::setCX(double x)
{
    _points[0].setX(x);
}

void CEllipse::setCY(double y)
{
    _points[0].setY(y);
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
    const CPoint & cp = _points[0];

    CBezier * b1 = drawBezierEllipseQuarter(cp, QSizeF(_radius.first, _radius.second));
    CBezier * b2 = drawBezierEllipseQuarter(cp, QSizeF(-_radius.first, _radius.second));
    CBezier * b3 = drawBezierEllipseQuarter(cp, QSizeF(-_radius.first, -_radius.second));
    CBezier * b4 = drawBezierEllipseQuarter(cp, QSizeF(_radius.first, -_radius.second));

    b2->reverse();
    b4->reverse();

    path->addNext(b1);
    path->addNext(b2);
    path->addNext(b3);
    path->addNext(b4);
    addNext(path);

    return true;
}
