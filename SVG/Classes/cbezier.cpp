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
    CPrimitive(PT_BEZIER, s, a1, a2, e)
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

/**
* @brief Сделаем параллельную кривую на заданном расстоянии
* @param d
* @ref https://pomax.github.io/bezierjs/
* @return
*/
QList<CBezier *> CBezier::makeOffset(double d)
{

    //make dpoints
}

/**
* @brief Вычисляем направление по/против часовой
* @return true if clockwise
*/
bool CBezier::direction()
{

}

CPoint CBezier::get(double t)
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

CPoint CBezier::normal(double t)
{
    CPoint d = derivative(t);
    double q = sqrt(d.x()*d.x() + d.y()*d.y());
    return CPoint(-d.y()/q, d.x()/q);
}

/**
* @brief Вычисляем производную
* @param t
* @return
*/
CPoint CBezier::derivative(double t)
{
    double mt = 1 - t, a = 0, b = 0, c = 0;

    CPoints p = dpoints[0];

    a = mt * mt;
    b = mt * t * 2;
    c = t * t;

    CPoint ret(a*p[0].x()+b*p[1].x()+c*p[2].x(), a*p[0].y()+b*p[1].y()+c*p[2].y());

    return ret;
}

CBezier::TOffset CBezier::boffset(double t, double d)
{
    CPoint c = get(t);
    CPoint n = normal(t);
    CPoint e = CPoint(c.x()+n.x()*d, c.y()+n.y()*d);
    TOffset ret = {c, n, e};
    return ret;
}

QList<CBezier *> CBezier::boffset(double d)
{
    QList<CBezier*> retList;
    bool linear = false;

    if ( linear ) {
        CPoint nv = normal(0);

        CPoints np;
        foreach(CPoint p, _points) {
            np.add(CPoint(p.x()+d*nv.x(), p.y()+d*nv.y()));
        }

        return { new CBezier(np) };
    }

    /*var reduced = reduce();

    return reduced.map(function(s) {
      /*if (s._linear) {
        return s.boffset(d)[0];
      }
      return s.bscale(d);
    });*/
}

/**
* @brief Маштабируем
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
    CPoints np;

    //-- Move end points by fixed distance along normal.
    for (int t=0; t<2; t++) {
        np[t * order] = points[t * order];
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


void CBezier::reduce()
{
/*    double step = 0.01;
    QList<CBezier *> pass1;
    QList<CBezier *> pass2;

    // first pass: split on extrema
    QList<double> extrema = bextrema().roots;
    if ( extrema.indexOf(0)==-1 ) {
      extrema.prepend(0);
    }
    if ( extrema.indexOf(1)==-1 ) {
      extrema.append(1);
    }

    for (int t1 = extrema[0], i = 1; i<extrema.count(); ++i) {
      int t2 = extrema[i];
      CBezier * segment = split(t1, t2);
      pass1.append(segment);
      t1 = t2;
    }

    // second pass: further reduce these segments to simple segments
    foreach(CBezier * p1, pass1) {
      int t1 = 0;
      int t2 = 0;
      while (t2 <= 1) {
        for (t2 = t1 + step; t2 <= 1 + step; t2 += step) {
          CBezier * segment = p1.split(t1, t2);
          if ( !segment.simple() ) {
            t2 -= step;
            if ( abs(t1 - t2)<step ) {
              return [];
            }
            segment = p1.split(t1, t2);
            segment._t1 = umap(t1, 0, 1, p1._t1, p1._t2);
            segment._t2 = umap(t2, 0, 1, p1._t1, p1._t2);
            pass2.push(segment);
            t1 = t2;
            break;
          }
        }
      }
      if ( t1<1 ) {
        segment = p1.split(t1, 1);
        segment._t1 = utils.map(t1, 0, 1, p1._t1, p1._t2);
        segment._t2 = p1._t2;
        pass2.push(segment);
      }
    });
    return pass2;*/
}

CBezier::TExtrema CBezier::bextrema()
{
    TExtrema ret;

    for (int c=0; c<2; ++c) {
        QList<double> p0;
        foreach (CPoint dp, dpoints[0]) {
            switch (c) {
                case 0: p0.append(dp.x()); break;
                case 1: p0.append(dp.y()); break;
            }
        }
        ret.result[c] = droots(p0);

        QList<double> p1;
        foreach (CPoint dp, dpoints[1]) {
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
    for (int idx=0; idx<=ret.roots.count(); ++idx) {
        if ( ret.roots.indexOf(ret.roots[idx])==idx ) { rootsFiltered.append(ret.roots[idx]); }
    }

    ret.roots = rootsFiltered;

    return ret;
}


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

QList<double> CBezier::droots(const QList<double> &p)
{
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
}

CPoints CBezier::hull(double t)
{
    CPoints p = _points;
    CPoints q;

    int idx = 0;
    q[idx++] = p[0];
    q[idx++] = p[1];
    q[idx++] = p[2];
    q[idx++] = p[3];

    //-- We lerp between all points at each iteration, until we have 1 point left.
    while ( p.count()>1 ) {
        CPoints _p;
        for (int i=0, l=p.count()-1; i<l; ++i) {
            CPoint pt = lerp(t, p[i], p[i+1]);
            q[idx++] = pt;
            _p.append(pt);
        }
        p = _p;
    }

    return q;
}

CPoint CBezier::lerp(double r, const CPoint &v1, const CPoint &v2) const
{
    CPoint ret(v1.x()+r*(v2.x() - v1.x()), v1.y() + r*(v2.y() - v1.y()));
    return ret;
}

CBezier::TSplit CBezier::split(double t1, double t2) const
{
/*    //-- Shortcuts
    if ( t1==0 && t2==t2) {
        //return split(t2).first;
    }
    if ( t2==1 ) {
        //return split(t1).second;
    }

    //-- No shortcut: use "de Casteljau" iteration.
    CPoints q = hull(t1);

    TSplit result {new CBezier([q[0], q[4], q[7], q[9]]), new Bezier([q[9], q[8], q[6], q[3]]), q};

    // make sure we bind _t1/_t2 information!
    result.left._t1 = utils.map(0, 0, 1, this._t1, this._t2);
    result.left._t2 = utils.map(t1, 0, 1, this._t1, this._t2);
    result.right._t1 = utils.map(t1, 0, 1, this._t1, this._t2);
    result.right._t2 = utils.map(1, 0, 1, this._t1, this._t2);

    // if we have no t2, we're done
    if ( t2!=t2 ) {
        return result;
    }

    // if we have a t2, split again:
    t2 = utils.map(t2, t1, 1, 0, 1);
    var subsplit = result.right.split(t2);
    return subsplit.left;*/
}


