#include "ccircle.h"
#include "cpath.h"
#include "cbezier.h"
#include "cellipse.h"

CCircle::CCircle(): CPrimitive(PT_CIRCLE, CPoint())
{

}

CCircle::CCircle(const CCircle &other): CPrimitive(other)
{
    _radius = other._radius;
}


CCircle::CCircle(CPoint center, double radius): CPrimitive(PT_CIRCLE, center), _radius(radius)
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

    CBezier * b1 = CEllipse::drawBezierEllipseQuarter(center, QSize(-_radius, -_radius));
    CBezier * b2 = CEllipse::drawBezierEllipseQuarter(center, QSize(_radius, -_radius));
    CBezier * b3 = CEllipse::drawBezierEllipseQuarter(center, QSize(_radius, _radius));
    CBezier * b4 = CEllipse::drawBezierEllipseQuarter(center, QSize(-_radius, _radius));

    b2->reverse();
    b4->reverse();

    path->addNext(b1);
    path->addNext(b2);
    path->addNext(b3);
    path->addNext(b4);
    addNext(path);

    path->setIsClosed(true);

    return true;
}

CBoundingBox CCircle::boundingBox(bool withTransform) const
{
    if ( !_boundingBox.isEmpty() ) { return _boundingBox; }

    CPoint R(_radius, _radius);
    _boundingBox.addPoint(_points.p1()-R);
    _boundingBox.addPoint(_points.p1()+R);

    if ( withTransform ) { _boundingBox.transform(_transformMatrix); }

    boundingBoxChilds(withTransform);

    return _boundingBox;
}
