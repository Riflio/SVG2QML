#include "cbezier.h"

CBezier::CBezier()
{

}

/**
* @brief Кривая Безье
* @param s - начальная точка
* @param a1 - первый якорь
* @param a2 - второй якорь
* @param e - конечная точка
*/
CBezier::CBezier(CPoint s, CPoint a1, CPoint a2, CPoint e):
    CPrimitive(CPrimitive::PT_BEZIER, s, a1, a2, e)
{

}

CBezier::CBezier(const CBezier &other):
    CPrimitive(other)
{

}

void CBezier::cpaint(QPainter *painter, const CBoundingBox &area)
{
    QPainterPath path;

    const CPoint &p1 =  _points.p1();
    const CPoint &p2 =  _points.p2();
    const CPoint &p3 =  _points.p3();
    const CPoint &p4 =  _points.p4();

    path.moveTo(p1.x(), p1.y());
    path.cubicTo(p2.x(), p2.y(),  p3.x(), p3.y(),  p4.x(), p4.y());

    drawPath(path, painter);
}

void CBezier::cstream(QDataStream &dataStream, double scale)
{
    dataStream<<PT_BEZIER<<_points.p1()*scale<<_points.p2()*scale<<_points.p3()*scale<<_points.p4()*scale;
}

/**
* @brief Выродились тупо в линию
* @param tol - насколько точно
* @return
*/
bool CBezier::isFlat(double tol) const
{
    tol = 16*tol*tol;

    const CPoint &p1 = _points.p1();
    const CPoint &c1 = _points.p2();
    const CPoint &c2 = _points.p3();
    const CPoint &p2 = _points.p4();

    double ux = 3*c1.x() - 2*p1.x() - p2.x();
    double uy = 3*c1.y() - 2*p1.y() - p2.y();
    double vx = 3*c2.x() - 2*p2.x() - p1.x();
    double vy = 3*c2.y() - 2*p2.y() - p1.y();

    ux *= ux;
    uy *= uy;
    vx *= vx;
    vy *= vy;

    if ( ux<vx ) { ux = vx; }
    if ( uy<vy ) { uy = vy; }

    return ( ux+uy<=tol );

}

/**
* @brief Упрощаем до набора отрезков
* @param tol - насколько точно
* @return
*/
CPoints CBezier::lianirize(double tol) const
{
    CPoints finished;
    finished.append(_points.p1());

    QVector<CBezier> todo = { *static_cast<CBezier*>(this->copy()) };

    while ( todo.count()>0 ){
        const CBezier & segment = todo[0];
        if( segment.isFlat(tol) ) {
            finished.append(segment.points().p4());
            todo.takeFirst();
        } else {
            QPair<CBezier,CBezier> divided = segment.subDivide(0.5);
            todo.takeFirst();
            todo.append(divided.first);
            todo.append(divided.second);
        }
    }

    return finished;
}

/**
* @brief Переопределяем ограничительную рамку
* @return
*/
const CBoundingBox &CBezier::getBBox() const
{
    if ( !_bbox.isEmpty() ) return _bbox;

    const CPoint &p1 = _points.p1();
    const CPoint &p2 = _points.p2();
    const CPoint &p3 = _points.p3();
    const CPoint &p4 = _points.p4();

    QVector<double> PX = {p1.x(), p2.x(), p3.x(), p4.x()};
    QVector<double> PY = {p1.y(), p2.y(), p3.y(), p4.y()};

    double a = 3 * p4.x() - 9 * p3.x() + 9 * p2.x() - 3 * p1.x();
    double b = 6 * p1.x() - 12 * p2.x() + 6 * p3.x();
    double c = 3 * p2.x() - 3 * p1.x();

    double disc = b * b - 4 * a * c;
    double xl = p1.x();
    double xh = p1.x();
    if ( p4.x()<xl ) { xl = p4.x(); }
    if ( p4.x()>xh ) { xh = p4.x(); }
    if ( disc>=0 ) {
        double t1 = (-b+sqrt(disc))/(2*a);
        if ( t1>0 && t1<1 ) {
            double x1 = evalBez(PX, t1);
            if ( x1<xl ) { xl = x1; }
            if ( x1>xh ) { xh = x1; }
        }

        double t2 = (-b-sqrt(disc))/(2*a);
        if ( t2>0 && t2<1 ) {
            double x2 = evalBez(PX, t2);
            if ( x2<xl ) { xl = x2; }
            if ( x2>xh ) { xh = x2; }
        }
    }

    a = 3 * p4.y() - 9 * p3.y() + 9 * p2.y() - 3 * p1.y();
    b = 6 * p1.y() - 12 * p2.y() + 6 * p3.y();
    c = 3 * p2.y() - 3 * p1.y();
    disc = b * b - 4 * a * c;

    double yl = p1.y();
    double yh = p1.y();
    if ( p4.y()<yl ) { yl = p4.y(); }
    if ( p4.y()>yh ) { yh = p4.y(); }
    if ( disc>=0 ) {
        double t1 = (-b+sqrt(disc))/(2*a);
        if ( t1>0 && t1<1 ) {
            double y1 = evalBez(PY, t1);
            if ( y1<yl ) { yl = y1; }
            if ( y1>yh ) { yh = y1; }
        }

        double t2 = (-b-sqrt(disc))/(2*a);
        if ( t2>0 && t2<1) {
            double y2 = evalBez(PY, t2);
            if (y2 < yl) yl = y2;
            if (y2 > yh) yh = y2;
        }
    }

    _bbox.addPoint(CPoint(xl, yl));
    _bbox.addPoint(CPoint(xh, yh));

    return _bbox;
}

/**
* @brief Разделяем на две
* @param t - где именно в процентном соотношении по длине [0-1]
* @return
*/
QPair<CBezier,CBezier> CBezier::subDivide(double t) const
{
    const CPoint &p1 =  _points.p1();
    const CPoint &c1 =  _points.p2();
    const CPoint &c2 =  _points.p3();
    const CPoint &p2 =  _points.p4();

    CPoint mid1(p1.x()+(c1.x()-p1.x())*t, p1.y()+(c1.y()-p1.y())*t);
    CPoint mid2(c2.x()+(p2.x()-c2.x())*t, c2.y()+(p2.y()-c2.y())*t);
    CPoint mid3(c1.x()+(c2.x()-c1.x())*t, c1.y()+(c2.y()-c1.y())*t);
    CPoint mida(mid1.x()+(mid3.x()-mid1.x())*t, mid1.y()+(mid3.y()-mid1.y())*t);
    CPoint midb(mid3.x()+(mid2.x()-mid3.x())*t, mid3.y()+(mid2.y()-mid3.y())*t);
    CPoint midx(mida.x()+(midb.x()-mida.x())*t, mida.y()+(midb.y()-mida.y())*t);

    CBezier d1(p1, mid1, mida, midx);
    CBezier d2(midx, midb, mid2, p2);

    return QPair<CBezier,CBezier>(d1, d2);
}

double CBezier::evalBez(const QVector<double> poly, double t) const
{
    double x = poly[0]*(1-t)*(1-t)*(1-t)+3*poly[1]*t*(1-t)*(1-t)+3*poly[2]*t*t*(1-t)+poly[3]*t*t*t;
    return x;
}
