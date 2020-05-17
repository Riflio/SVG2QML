#include "cpoints.h"

CPoints::CPoints(): QVector<CPoint>()
{
}

CPoints::CPoints(const QVector<CPoint> &points): QVector<CPoint>(points)
{
}

const CPoint &CPoints::p1() const
{
    return at(0);
}

const CPoint &CPoints::p2() const
{
    return at(1);
}

const CPoint &CPoints::p3() const
{
    return at(2);
}

const CPoint &CPoints::p4() const
{
    return at(3);
}

const CPoint &CPoints::p5() const
{
    return at(4);
}

const CPoint &CPoints::p6() const
{
    return at(5);
}

void CPoints::add(const CPoint &p)
{
    append(p);
}

void CPoints::reverse()
{
    std::reverse(begin(), end());
}

void CPoints::move(const CPoint &d)
{
    for(int i=0; i<count(); ++i) {
        (operator [](i)).add(d);
    }
}

/**
* @brief Вращаем на заданный угол
* @param center - вокруг какой точки вращаем
* @param degrees - угол в радианах
*/
void CPoints::rotate(const CPoint &center, double degrees)
{
    double angle = degrees;
    double ccos = cos(angle);
    double ssin = sin(angle);

    for(int i=0; i<count(); ++i) {
        CPoint & p =(operator [](i));        

        double nx = (ccos * (p.x() - center.x())) + (ssin * (p.y() - center.y())) + center.x();
        double ny = (ccos * (p.y() - center.y())) - (ssin * (p.x() - center.x())) + center.y();

        p.set(nx, ny);
    }
}
