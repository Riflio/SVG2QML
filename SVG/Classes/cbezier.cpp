#include "cbezier.h"

CBezier::CBezier()
{

}

/**
* @brief Cubic Bezier Curve
* @param s - start point
* @param a1 - anchor 1
* @param a2 - anchor 2
* @param e - end point
*/
CBezier::CBezier(const CPoint &s, const CPoint &a1, const CPoint &a2, const CPoint &e):
    CPrimitive(PT_BEZIER, s, a1, a2, e)
{

}

/**
* @brief Quadratic Bezier curver
* @param s - start point
* @param a - anchor
* @param e - end point
*/
CBezier::CBezier(const CPoint &s, const CPoint &a, const CPoint &e):
    CPrimitive(PT_BEZIER, s, (s+(a-s)*(2.0/3.0)), (e+(a-e)*(2.0/3.0)), e)
{

}

CBezier::CBezier(const CBezier &other):
    CPrimitive(other)
{

}

CBezier::CBezier(const CPoints &points):
    CPrimitive(PT_BEZIER, points)
{

}

CBezier& CBezier::operator =(const CBezier& b)
{
    _type = b.type();
    _points = b.points();
    _styles = b.styles();
    _id = b.ID();
    _className = b.className();
    _transformMatrix = b.transformation();
    _title = b.title();
    _descr = b.descr();
    _boundingBox = b.boundingBox();
    return *this;
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

    QVector<CBezier> todo = { *static_cast<CBezier*>(this->clone()) };

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
CBoundingBox CBezier::boundingBox(bool withTransform) const
{
    if ( !_boundingBox.isEmpty() ) { return _boundingBox; }

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

    _boundingBox.addPoint(CPoint(xl, yl));
    _boundingBox.addPoint(CPoint(xh, yh));


    if ( withTransform ) { _boundingBox.transform(_transformMatrix); }

    return _boundingBox;
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

/**
* @brief Сделаем параллельную кривую на заданном расстоянии
* @param d
* @ref https://pomax.github.io/bezierjs/
* @return
*/
QList<CBezier *> CBezier::makeOffset(double d) const
{
    QList<CBezier *> list;
    list = boffset(d);
    return list;
}

/**
* @brief Вычисляем направление по/против часовой
* @return true if clockwise
*/
bool CBezier::direction()
{
    //TODO: Вычисляем направление по/против часовой
    return false;
}

/**
* @brief Получаем точку на кривой
* @ref https://pomax.github.io/bezierjs/#get
* @param t position 0-1
* @return
*/
CPoint CBezier::get(double t) const
{
    if ( t==0) { return _points[0]; }
    if ( t==1 ) { return _points[3]; }

    CPoints p = _points;
    double mt = 1 - t;
    double mt2 = mt * mt;
    double t2 = t * t, a = 0, b = 0, c = 0, d = 0;

    a = mt2 * mt;
    b = mt2 * t * 3;
    c = mt * t2 * 3;
    d = t * t2;

    CPoint ret(a * p[0].x() + b*p[1].x() + c*p[2].x() + d*p[3].x(), a * p[0].y() + b * p[1].y() + c * p[2].y() + d * p[3].y() );
    return ret;
}

/**
* @brief CBezier::normal
* @param t
* @return
*/
CPoint CBezier::normal(double t) const
{
    CPoint d = derivative(t);
    double q = sqrt(d.x()*d.x() + d.y()*d.y());
    return CPoint(-d.y()/q, d.x()/q);
}

/**
* @brief Вычисляем производную
* @ref https://pomax.github.io/bezierjs/#derivative
* @param t
* @return
*/
CPoint CBezier::derivative(double t) const
{
    double mt = 1 - t, a = 0, b = 0, c = 0;

    CPoints p = dPoints()[0];

    a = mt * mt;
    b = mt * t * 2;
    c = t * t;

    CPoint ret(a*p[0].x()+b*p[1].x()+c*p[2].x(), a*p[0].y()+b*p[1].y()+c*p[2].y());

    return ret;
}

/**
* @brief CBezier::boffset
* @ref https://pomax.github.io/bezierjs/#offset
* @param t
* @param d
* @return
*/
CBezier::TOffset CBezier::boffset(double t, double d)
{
    CPoint c = get(t);
    CPoint n = normal(t);
    CPoint e = CPoint(c.x()+n.x()*d, c.y()+n.y()*d);
    TOffset ret = {c, n, e};
    return ret;
}

/**
* @brief CBezier::boffset
* @ref https://pomax.github.io/bezierjs/#offset
* @param d
* @return
*/
QList<CBezier *> CBezier::boffset(double d) const
{
    bool linear = false;

    if ( linear ) {
        CPoint nv = normal(0);

        CPoints np;
        foreach(CPoint p, _points) {
            np.add(CPoint(p.x()+d*nv.x(), p.y()+d*nv.y()));
        }

        return { new CBezier(np) };
    }

    QList<CBezier*> retList;
    QList<CBezier *> reduced = reduce();

    foreach (CBezier * s, reduced) {
        retList.append(s->bscale(d));
    }

    return retList;
}

/**
* @brief Маштабируем
* @ref https://pomax.github.io/bezierjs/#scale
* @param d
* @return
*/
CBezier * CBezier::bscale(double d)
{
    int order = 3;
    double r1 = d;
    double r2 = d;
    bool distanceFn = false;

    QList<TOffset> v = { boffset(0, 10), boffset(1, 10) };
    CPoint o = lli4(v[0].e, v[0].c, v[1].e, v[1].c);

    if ( o.isEmpty() ) {
        throw 11; //-- Cannot scale this curve. Try reducing it first.
    }

    //-- Move all points by distance 'd' wrt the origin 'o'
    CPoints points = _points;
    CPoints np = _points;

    //-- Move end points by fixed distance along normal.
    for (int t=0; t<2; t++) {
        np[t * order].incX( ((t==1)? r2 : r1) * v[t].n.x());
        np[t * order].incY( ((t==1)? r2 : r1) * v[t].n.y());
    }

    if ( !distanceFn ) { //-- Move control points to lie on the intersection of the offset derivative vector, and the origin-through-control vector
        for(int t=0; t<2; ++t) {
          CPoint p = np[t * order];
          CPoint d = derivative(t);
          CPoint p2 (p.x() + d.x(), p.y() + d.y());

          np[t + 1] = lli4(p, p2, o, points[t+1]);
        }

        return new CBezier(np);
    }


    //-- Move control points by "however much necessary to ensure the correct tangent to endpoint".
    for (int t=0; t<2; ++t) {
        CPoint p = points[t + 1];
        CPoint ov(p.x() - o.x(), p.y() - o.y());
        double rc = d;
        double m = sqrt(ov.x() * ov.x() + ov.y() * ov.y());
        ov.divide(m);
        np[t + 1] = CPoint(p.x() + rc * ov.x(), p.y() + rc * ov.y());
    };

    return new CBezier(np);
}

/**
* @brief CBezier::reduce
* @ref https://pomax.github.io/bezierjs/#reduce
* @return
*/
QList<CBezier *> CBezier::reduce() const
{
    double step = 0.01;
    QList<CBezier *> pass1;
    QList<CBezier *> pass2;

    //-- First pass: split on extrema
    QList<double> extr = extrema().roots;
    if ( extr.indexOf(0)==-1 ) {
      extr.prepend(0);
    }
    if ( extr.indexOf(1)==-1 ) {
      extr.append(1);
    }

    double t1 = extr[0];
    for (int i = 1; i<extr.count(); ++i) {
      double t2 = extr[i];
      CBezier * segment = split(t1, t2)[0];
      pass1.append(segment);
      t1 = t2;
    }

    //-- Second pass: further reduce these segments to simple segments
    foreach(CBezier * p1, pass1) {
        double t1 = 0;
        double t2 = 0;
        bool exStop = false;
        while (t2 <= 1) {
            for (t2 = t1 + step; t2 <= 1 + step; t2 += step) {
                CBezier * segment = p1->split(t1, t2)[0];
                if ( !segment->simple() ) {
                    t2 -= step;
                    if ( abs(t1-t2)<step ) { exStop = true; break; }
                    segment = p1->split(t1, t2)[0];
                    segment->_t1 = map(t1, 0, 1, p1->_t1, p1->_t2);
                    segment->_t2 = map(t2, 0, 1, p1->_t1, p1->_t2);
                    pass2.append(segment);
                    t1 = t2;
                    break;
                }
            }

            if ( exStop ) break;
        }

        if ( exStop ) continue;

        if ( t1<1 ) {
            CBezier * segment = p1->split(t1, 1)[0];
            segment->_t1 = map(t1, 0, 1, p1->_t1, p1->_t2);
            segment->_t2 = p1->_t2;
            pass2.append(segment);
        }
    }

    return pass2;
}

/**
* @brief CBezier::extrema
* @ref https://pomax.github.io/bezierjs/#extrema
* @return
*/
CBezier::TExtrema CBezier::extrema() const
{
    TExtrema ret;

    dPoints();

    for (int c=0; c<2; ++c) {
        QList<double> p0;
        foreach (CPoint dp, _dpoints[0]) {
            switch (c) {
                case 0: p0.append(dp.x()); break;
                case 1: p0.append(dp.y()); break;
            }
        }
        ret.result[c] = droots(p0);

        QList<double> p1;
        foreach (CPoint dp, _dpoints[1]) {
            switch (c) {
                case 0: p1.append(dp.x()); break;
                case 1: p1.append(dp.y()); break;
            }
        }
        ret.result[c].append(droots(p1));

        QList<double> filtered;
        std::copy_if(ret.result[c].begin(), ret.result[c].end(), std::back_inserter(filtered), [](double t){ return t >= 0 && t <= 1; } );

        ret.result[c] = filtered;

        std::sort(filtered.begin(), filtered.end());
        ret.roots.append(filtered);
    };

    std::sort(ret.roots.begin(), ret.roots.end());

    QList<double> rootsFiltered;
    for (int idx=0; idx<ret.roots.count(); ++idx) {
        if ( ret.roots.indexOf(ret.roots[idx])==idx ) { rootsFiltered.append(ret.roots[idx]); }
    }

    ret.roots = rootsFiltered;

    return ret;
}

/**
* @brief CBezier::lli4
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/utils.js
* @param p1
* @param p2
* @param p3
* @param p4
* @return
*/
CPoint CBezier::lli4(const CPoint &p1, const CPoint &p2, const CPoint &p3, CPoint const &p4) const
{
    double nx = (p1.x() * p2.y() - p1.y() * p2.x()) * (p3.x() - p4.x()) - (p1.x() - p2.x()) * (p3.x() * p4.y() - p3.y() * p4.x());
    double ny = (p1.x() * p2.y() - p1.y() * p2.x()) * (p3.y() - p4.y()) - (p1.y() - p2.y()) * (p3.x() * p4.y() - p3.y() * p4.x());
    double d = (p1.x() - p2.x()) * (p3.y() - p4.y()) - (p1.y() - p2.y()) * (p3.x() - p4.x());

    if ( d==0 ) {
        return CPoint();
    }

    return CPoint(nx/d, ny/d);
}

/**
* @brief CBezier::droots
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/utils.js
* @param p
* @return
*/
QList<double> CBezier::droots(const QList<double> &p) const
{
    if ( p.count()==3 ) {
        double a = p[0], b = p[1], c = p[2];
        double d = a - 2 * b + c;

        if (d != 0) {
            double m1 = -sqrt(b*b - a*c);
            double m2 = -a + b;
                double v1 = -(m1+m2) / d;
                double v2 = -(-m1+m2) / d;
                return {v1, v2};
        } else
        if ( b!=c && d==0 ) {
            return {(2*b-c) / (2*(b-c))};
        }

        return {};
    } else
    if ( p.count()==2 ) {
        double a = p[0];
        double b = p[1];
        if ( !Equal::almostEqual(a, b) ) {
            return {a / (a - b)};
        }
        return {};
    }

    return {};
}

/**
* @brief CBezier::hull
* @ref https://pomax.github.io/bezierjs/#hull
* @param t
* @return
*/
CPoints CBezier::hull(double t) const
{
    CPoints p = _points;
    CPoints q;
    q.reserve(4);

    q.add(p[0]);
    q.add(p[1]);
    q.add(p[2]);
    q.add(p[3]);

    //-- We lerp between all points at each iteration, until we have 1 point left.
    while ( p.count()>1 ) {
        CPoints _p;
        for (int i=0, l=p.count()-1; i<l; ++i) {
            CPoint pt = lerp(t, p[i], p[i+1]);
            q.add(pt);
            _p.append(pt);
        }
        p = _p;
    }

    return q;
}

/**
* @brief CBezier::lerp
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/utils.js
* @param r
* @param v1
* @param v2
* @return
*/
CPoint CBezier::lerp(double r, const CPoint &v1, const CPoint &v2) const
{
    CPoint ret(v1.x()+r*(v2.x() - v1.x()), v1.y() + r*(v2.y() - v1.y()));
    return ret;
}

/**
* @brief CBezier::split
* @ref https://pomax.github.io/bezierjs/#split
* @param t1
* @param t2
* @return
*/
QList<CBezier*> CBezier::split(double t1, double t2) const
{
    //-- Shortcuts
    if ( t1==0 && t2>=0) {
        return { split(t2)[0] };
    }
    if ( t2==1 ) {
        return { split(t1)[1] };
    }

    //-- No shortcut: use "de Casteljau" iteration.
    CPoints q = hull(t1);

    QList<CBezier *> result { new CBezier({q[0], q[4], q[7], q[9]}), new CBezier({q[9], q[8], q[6], q[3]}) };

    //-- Make sure we bind _t1/_t2 information!
    result[0]->_t1 = map(0, 0, 1, _t1, _t2);
    result[0]->_t2 = map(t1, 0, 1, _t1, _t2);
    result[1]->_t1 = map(t1, 0, 1, _t1, _t2);
    result[1]->_t2 = map(1, 0, 1, _t1, _t2);

    //-- If we have no t2, we're done
    if ( t2<0 ) {
        return result;
    }

    //-- If we have a t2, split again:
    t2 = map(t2, t1, 1, 0, 1);
    result = result[1]->split(t2);
    return result;
}

/**
* @brief CBezier::map
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/utils.js
* @param v
* @param ds
* @param de
* @param ts
* @param te
* @return
*/
double CBezier::map(double v, double ds, double de, double ts, double te) const
{
    double d1 = de - ds;
    double d2 = te - ts;
    double v2 = v - ds;
    double r = v2 / d1;
    return ts + d2 * r;
}

/**
* @brief CBezier::simple
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/bezier.js
* @return
*/
bool CBezier::simple() const
{
    double a1 = _points[0].angle(_points[3], _points[1]);
    double a2 = _points[0].angle(_points[3], _points[2]);
    if ((a1 > 0 && a2 < 0) || (a1 < 0 && a2 > 0)) return false;

    CPoint n1 = normal(0);
    CPoint n2 = normal(1);
    double s = n1.x()*n2.x() + n1.y()* n2.y();
    double angle = abs(acos(s));
    return angle < M_PI / 3;
}

/**
* @brief
* @ref https://github.com/Pomax/bezierjs/blob/master/lib/utils.js
* @param points
* @return
*/
QList<CPoints> CBezier::derive(const CPoints &points) const
{
    QList<CPoints> dpoints;

    CPoints p = points;
    for (int d = p.count(), c = d-1; d>1; --d, --c) {
        CPoints list;
        for (int j = 0; j<c; ++j) {
            CPoint dpt(c*(p[j+1].x() - p[j].x()),  c*(p[j+1].y() - p[j].y()));
            list.add(dpt);
        }
        dpoints.append(list);
        p = list;
    }

    return dpoints;
}

/**
* @brief CBezier::dPoints
* @return
*/
QList<CPoints> CBezier::dPoints() const
{
    if ( _dpoints.count()==0 ) { _dpoints = derive(_points); }
    return _dpoints;
}


