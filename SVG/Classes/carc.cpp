#include "carc.h"
#include "math.h"
#include "Algebra/equal.h"
#include "Algebra/cvector2d.h"

CArc::CArc()
    :CPrimitive(PT_ARC)
{

}

/**
* @brief Дуга
* @param startPoint - координаты начальной точки
* @param rx - радиус по X
* @param ry - радиус по Y
* @param rotation - угол поворота по X
* @param largeArc - выводить большую часть дуги
* @param sweep - направление отрисовки (по часовой/против)
* @param endPoint - координаты конечной точки
*/
CArc::CArc(CPoint startPoint, double rx, double ry, double rotation, bool largeArc, bool sweep, CPoint endPoint)
    :CPrimitive(CPrimitive::PT_ARC, startPoint, endPoint), _rx(rx), _ry(ry),
    _rotation(rotation), _largeArc(largeArc), _sweep(sweep)
{

}

double CArc::rx() const
{
    return _rx;
}

double CArc::ry() const
{
    return _ry;
}

double CArc::rotation() const
{
    return _rotation;
}

bool CArc::largeArcFlag() const
{
    return _largeArc;
}

bool CArc::sweepFlag() const
{
    return _sweep;
}

/**
* @brief Преобразуем в путь
* @return
* @ref https://github.com/colinmeinke/svg-arc-to-cubic-bezier
* @ref https://ru.wikipedia.org/wiki/%D0%9A%D1%80%D0%B8%D0%B2%D0%B0%D1%8F_%D0%91%D0%B5%D0%B7%D1%8C%D0%B5
*/
bool CArc::toPath()
{
    if ( (_rx==0) || (_ry==0) ) return false;

    CPath * path = new CPath();
    CPoint s = _points.p1();
    CPoint e = _points.p2();

    double sinphi = sin( _rotation * TAU / 360 );
    double cosphi = cos( _rotation * TAU / 360 );

    double pxp = cosphi * (s.x() - e.x()) / 2 + sinphi * (s.y() - e.y()) / 2;
    double pyp = -sinphi * (s.x() - e.x()) / 2 + cosphi * (s.y() - e.y()) / 2;

    if ( Equal::almostEqual(pxp, 0) && Equal::almostEqual(pyp, 0) ) { return false; }

    double rx = abs(_rx);
    double ry = abs(_ry);

    double lambda = pow(pxp, 2) / pow(rx, 2) + pow(pyp, 2) / pow(ry, 2);

    if ( lambda>1 ) {
        rx *= sqrt(lambda);
        ry *= sqrt(lambda);
    }

    TArcCenter arcCenter = getArcCenter(s, e, rx, ry, _largeArc, _sweep, sinphi, cosphi, pxp, pyp);

    int segments = qMax(ceil(abs(arcCenter.ang2) / (TAU / 4)), 1.0);
    arcCenter.ang2 /= (double)segments;

    QList<QList<CPoint>> curves;
    for (int i = 0; i<segments; ++i) {
        curves.append(approxUnitArc(arcCenter.ang1, arcCenter.ang2));
        arcCenter.ang1 += arcCenter.ang2;
    }

    CPoint p1 = s;
    foreach(QList<CPoint> curve, curves) {
        CPoint p2 = mapToEllipse(curve[0], rx, ry, cosphi, sinphi, arcCenter.p);
        CPoint p3 = mapToEllipse(curve[1], rx, ry, cosphi, sinphi, arcCenter.p);
        CPoint p4 = mapToEllipse(curve[2], rx, ry, cosphi, sinphi, arcCenter.p);
        CBezier * b = new CBezier(p1, p2, p3, p4);
        CNodeInterface::addNext(path, b);
        p1 = p4;
    }

    CNodeInterface::addNext(this, path);

    return true;
}

CArc::TArcCenter CArc::getArcCenter(CPoint s, CPoint e, double rx, double ry, bool largeArc, bool sweep, double sinphi, double cosphi, double pxp, double pyp) const
{
    double rxsq = pow(rx, 2);
    double rysq = pow(ry, 2);
    double pxpsq = pow(pxp, 2);
    double pypsq = pow(pyp, 2);
    double radicant = ( rxsq * rysq ) - ( rxsq * pypsq ) - ( rysq * pxpsq );
    if ( radicant<0 ) { radicant = 0; }
    radicant /= ( rxsq * pypsq ) + ( rysq * pxpsq );
    radicant = sqrt(radicant) * ((largeArc==sweep)? -1 : 1);
    double centerxp = radicant * rx/ry * pyp;
    double centeryp = radicant * -ry/rx * pxp;

    double centerX = cosphi * centerxp - sinphi * centeryp + ( s.x() + e.x() ) / 2;
    double centerY = sinphi * centerxp + cosphi * centeryp + ( s.y() + e.y() ) / 2;

    double vx1 =  ( pxp - centerxp ) / rx;
    double vy1 =  ( pyp - centeryp ) / ry;
    double vx2 = ( -pxp - centerxp ) / rx;
    double vy2 = ( -pyp - centeryp ) / ry;

    double ang1 = CVector2D(1, 0).angle(CVector2D(vx1, vy1));
    double ang2 = CVector2D(vx1, vy1).angle(CVector2D(vx2, vy2));

    if ( !sweep && ang2>0 ) { ang2 -= TAU; }
    if ( sweep && ang2<0 ) { ang2 += TAU; }

    return {CPoint(centerX, centerY), ang1, ang2};
}

QList<CPoint> CArc::approxUnitArc(double ang1, double ang2) const
{
    double a = 4 / 3 * tan(ang2 / 4);
    double x1 = cos(ang1);
    double y1 = sin(ang1);
    double x2 = cos(ang1 + ang2);
    double y2 = sin(ang1 + ang2);


    QList<CPoint> l;
    l.append(CPoint(x1 - y1 * a, y1 + x1 * a));
    l.append(CPoint(x2 + y2 * a, y2 - x2 * a));
    l.append(CPoint(x2, y2));

    return l;
}

CPoint CArc::mapToEllipse(CPoint p, double rx, double ry, double cosphi, double sinphi, CPoint cp) const
{
    double x = p.x()*rx;
    double y = p.y()*ry;
    double xp = cosphi * x - sinphi * y;
    double yp = sinphi * x + cosphi * y;
    return CPoint(xp + cp.x(), yp + cp.y());
}
