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

/**
* @brief Сделаем параллельную кривую на заданном расстоянии
* @param d
* @ref https://pomax.github.io/bezierjs/
* @return
*/
QList<CBezier *> CBezier::makeOffset(double d)
{

}



/**
offset: function(t, d) {
      if (typeof d !== "undefined") {
        var c = this.get(t);
        var n = this.normal(t);
        var ret = {
          c: c,
          n: n,
          x: c.x + n.x * d,
          y: c.y + n.y * d
        };
        if (this._3d) {
          ret.z = c.z + n.z * d;
        }
        return ret;
      }
      if (this._linear) {
        var nv = this.normal(0);
        var coords = this.points.map(function(p) {
          var ret = {
            x: p.x + t * nv.x,
            y: p.y + t * nv.y
          };
          if (p.z && n.z) {
            ret.z = p.z + t * nv.z;
          }
          return ret;
        });
        return [new Bezier(coords)];
      }
      var reduced = this.reduce();
      return reduced.map(function(s) {
        if (s._linear) {
          return s.offset(t)[0];
        }
        return s.scale(t);
      });
    },
 reduce: function() {
      var i,
        t1 = 0,
        t2 = 0,
        step = 0.01,
        segment,
        pass1 = [],
        pass2 = [];
      // first pass: split on extrema
      var extrema = this.extrema().values;
      if (extrema.indexOf(0) === -1) {
        extrema = [0].concat(extrema);
      }
      if (extrema.indexOf(1) === -1) {
        extrema.push(1);
      }

      for (t1 = extrema[0], i = 1; i < extrema.length; i++) {
        t2 = extrema[i];
        segment = this.split(t1, t2);
        segment._t1 = t1;
        segment._t2 = t2;
        pass1.push(segment);
        t1 = t2;
      }

      // second pass: further reduce these segments to simple segments
      pass1.forEach(function(p1) {
        t1 = 0;
        t2 = 0;
        while (t2 <= 1) {
          for (t2 = t1 + step; t2 <= 1 + step; t2 += step) {
            segment = p1.split(t1, t2);
            if (!segment.simple()) {
              t2 -= step;
              if (abs(t1 - t2) < step) {
                // we can never form a reduction
                return [];
              }
              segment = p1.split(t1, t2);
              segment._t1 = utils.map(t1, 0, 1, p1._t1, p1._t2);
              segment._t2 = utils.map(t2, 0, 1, p1._t1, p1._t2);
              pass2.push(segment);
              t1 = t2;
              break;
            }
          }
        }
        if (t1 < 1) {
          segment = p1.split(t1, 1);
          segment._t1 = utils.map(t1, 0, 1, p1._t1, p1._t2);
          segment._t2 = p1._t2;
          pass2.push(segment);
        }
      });
      return pass2;
    },

scale: function(d) {
      var order = this.order;
      var distanceFn = false;
      if (typeof d === "function") {
        distanceFn = d;
      }
      if (distanceFn && order === 2) {
        return this.raise().scale(distanceFn);
      }

      // TODO: add special handling for degenerate (=linear) curves.
      var clockwise = this.clockwise;
      var r1 = distanceFn ? distanceFn(0) : d;
      var r2 = distanceFn ? distanceFn(1) : d;
      var v = [this.offset(0, 10), this.offset(1, 10)];
      var o = utils.lli4(v[0], v[0].c, v[1], v[1].c);
      if (!o) {
        throw new Error("cannot scale this curve. Try reducing it first.");
      }
      // move all points by distance 'd' wrt the origin 'o'
      var points = this.points,
        np = [];

      // move end points by fixed distance along normal.
      [0, 1].forEach(
        function(t) {
          var p = (np[t * order] = utils.copy(points[t * order]));
          p.x += (t ? r2 : r1) * v[t].n.x;
          p.y += (t ? r2 : r1) * v[t].n.y;
        }.bind(this)
      );

      if (!distanceFn) {
        // move control points to lie on the intersection of the offset
        // derivative vector, and the origin-through-control vector
        [0, 1].forEach(
          function(t) {
            if (this.order === 2 && !!t) return;
            var p = np[t * order];
            var d = this.derivative(t);
            var p2 = { x: p.x + d.x, y: p.y + d.y };
            np[t + 1] = utils.lli4(p, p2, o, points[t + 1]);
          }.bind(this)
        );
        return new Bezier(np);
      }

      // move control points by "however much necessary to
      // ensure the correct tangent to endpoint".
      [0, 1].forEach(
        function(t) {
          if (this.order === 2 && !!t) return;
          var p = points[t + 1];
          var ov = {
            x: p.x - o.x,
            y: p.y - o.y
          };
          var rc = distanceFn ? distanceFn((t + 1) / order) : d;
          if (distanceFn && !clockwise) rc = -rc;
          var m = sqrt(ov.x * ov.x + ov.y * ov.y);
          ov.x /= m;
          ov.y /= m;
          np[t + 1] = {
            x: p.x + rc * ov.x,
            y: p.y + rc * ov.y
          };
        }.bind(this)
      );
      return new Bezier(np);
    },
