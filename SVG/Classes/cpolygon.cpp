#include "cpolygon.h"
#include "Algebra/csegment.h"
#include "Algebra/equal.h"
#include "cpath.h"
#include "cline.h"

CPolygon::CPolygon():CPrimitive(PT_POLYGON)
{

}

CPolygon::CPolygon(const CPoints &points):CPrimitive(CPrimitive::PT_POLYGON)
{
    setPoints(points);
}

/**
* @brief Создаём из прямоугольника по двум точкам
* @param tl
* @param br
*/
CPolygon::CPolygon(const CPoint &tl, const CPoint &br):CPrimitive(CPrimitive::PT_POLYGON)
{
    CPoints points;
    points.add(tl);
    points.add(CPoint(br.x(), tl.y()));
    points.add(br);
    points.add(CPoint(tl.x(), br.y()));
    points.add(tl);
    setPoints(points);
}

void CPolygon::addPoint(const CPoint &p)
{
    _points.add(p);
}

void CPolygon::cpaint(QPainter *painter, const CBoundingBox &area)
{
    Q_UNUSED(area);
    if ( _points.count()==0 ) { return; }
    QPainterPath path;

    path.moveTo(_points.p1().x(), _points.p1().y() );
    for(int i=1; i<_points.count(); ++i) {
        const CPoint &p = _points[i];
        path.lineTo(p.x(), p.y());
    }

    if ( !isClosed() ) {
        path.lineTo(_points.p1().x(), _points.p1().y());
    }

    drawPath(path, painter);
}

CPoints CPolygon::lianirize(double tol) const
{
    Q_UNUSED(tol);
    CPoints res;
    foreach(const CPoint &p, _points) {
        res.append(p);
    }
    return res;
}

/**
* @brief Отдаём направленную площадь
* @return
*/
double CPolygon::area() const
{
    double S = 0;
    for (int i=0, j=_points.count()-1; i<_points.count(); j=i++) {
        S += (_points[j].x()+_points[i].x()) * (_points[j].y()-_points[i].y());
    }
    return 0.5*S;
}

/**
* @brief Последняя точка должна быть такой же, как и первая
* @return
*/
bool CPolygon::isClosed() const
{
    return ( _points.first()==_points.last() );
}

/**
* @brief Закрываем полигон
*/
void CPolygon::close()
{
    if ( !isClosed() ) {
        _points.add(_points.first());
    }
}

bool CPolygon::toPath()
{
    if ( _points.count()==0 ) { return false; }

    CPath * path = new CPath();

    CPoint prevPoint = _points[0];
    for(int i=1; i<_points.count(); ++i) {
        CLine * line = new CLine(prevPoint, _points[i]);
        CNodeInterface::addNext(path, line);
        prevPoint = _points[i];
    }


    path->setIsClosed(true);
    CNodeInterface::addNext(this, path);
    return true;
}

/**
* @brief Находится ли указанная точка внутри полигона
* @param point
* @return
*/
int CPolygon::pointInPolygon(const CPoint &point) const
{
    if( _points.count()<3 ) { return -1; }
    bool inside = false;
    double offsetx = 0;
    double offsety = 0;

    for (int i=0, j=_points.count()-1; i<_points.count(); j=i++) {
        double xi = _points[i].x() + offsetx;
        double yi = _points[i].y() + offsety;
        double xj = _points[j].x() + offsetx;
        double yj = _points[j].y() + offsety;

        CPoint pi(xi, yi);
        CPoint pj(xj, yj);
        if ( point.isEq(pi) ) { return -1; }

        if ( CSegment(pi, pj).onSegment(point) ) { return -1; }

        if ( Equal::almostEqual(xi, xj) && Equal::almostEqual(yi, yj) ) { //-- Игнорим слишком короткие отрезки
            continue;
        }

        bool intersect = ( ((yi > point.y()) != (yj > point.y())) && (point.x() < (xj - xi) * (point.y() - yi) / (yj - yi) + xi) );
        if ( intersect ) inside = !inside;
    }

    return (inside)? 1 : 0;
}

/**
* @brief А не тупо прямоугольник ли мы
* @return
*/
bool CPolygon::isRectangle(double tolerance) const
{
    Q_UNUSED(tolerance);
    for(int i=0; i<_points.count(); ++i){
        if ( !Equal::almostEqual(_points[i].x(), _bbox.tl().x()) && !Equal::almostEqual(_points[i].x(), _bbox.br().x()) ) {
            return false;
        }
        if ( !Equal::almostEqual(_points[i].y(), _bbox.tl().y()) && !Equal::almostEqual(_points[i].y(), _bbox.br().y()) ) {
            return false;
        }
    }

    return true;
}

/**
* @brief Определяем, пересекает ли полигон наш
* @param other
* @return
*/
bool CPolygon::intersect(const CPolygon &other) const
{
    CPoint offsetA = offset;
    CPoint offsetB = other.offset;

    CPolygon A(*this);
    CPolygon B(other);

    for (int i=0; i<pointsCount()-1; ++i) {
        for (int j=0; j<B.pointsCount()-1; ++j) {
            CPoint a1(A[i].x()+offsetA.x(), A[i].y()+offsetA.y());
            CPoint a2(A[i+1].x()+offsetA.x(), A[i+1].y()+offsetA.y());
            CPoint b1(B[j].x()+offsetB.x(), B[j].y()+offsetB.y());
            CPoint b2(B[j+1].x()+offsetB.x(), B[j+1].y()+offsetB.y());

            int prevBindex = (j==0)? B.pointsCount()-1 : j-1;
            int prevAindex = (i==0)? A.pointsCount()-1 : i-1;
            int nextBindex = (j+1==B.pointsCount()-1)? 0 : j+2;
            int nextAindex = (i+1==A.pointsCount()-1)? 0 : i+2;

            // go even further back if we happen to hit on a loop end point
            if ( B[prevBindex]==B[j] || (Equal::almostEqual(B[prevBindex].x(), B[j].x()) && Equal::almostEqual(B[prevBindex].y(), B[j].y())) ) {
                prevBindex = (prevBindex==0)? B.pointsCount()-1 : prevBindex-1;
            }

            if ( A[prevAindex]==A[i] || (Equal::almostEqual(A[prevAindex].x(), A[i].x()) && Equal::almostEqual(A[prevAindex].y(), A[i].y())) ) {
                prevAindex = (prevAindex==0)? A.pointsCount()-1 : prevAindex-1;
            }

            // go even further forward if we happen to hit on a loop end point
            if ( B[nextBindex]==B[j+1] || (Equal::almostEqual(B[nextBindex].x(), B[j+1].x()) && Equal::almostEqual(B[nextBindex].y(), B[j+1].y())) ) {
                nextBindex = (nextBindex==B.pointsCount()-1)? 0 : nextBindex+1;
            }

            if ( A[nextAindex]==A[i+1] || (Equal::almostEqual(A[nextAindex].x(), A[i+1].x()) && Equal::almostEqual(A[nextAindex].y(), A[i+1].y())) ) {
                nextAindex = (nextAindex==A.pointsCount()-1)? 0 : nextAindex+1;
            }

            CPoint a0(A[prevAindex].x() + offsetA.x(), A[prevAindex].y() + offsetA.y());
            CPoint b0(B[prevBindex].x() + offsetB.x(), B[prevBindex].y() + offsetB.y());

            CPoint a3(A[nextAindex].x() + offsetA.x(), A[nextAindex].y() + offsetA.y());
            CPoint b3(B[nextBindex].x() + offsetB.x(), B[nextBindex].y() + offsetB.y());

            if ( CSegment(a1,a2).onSegment(b1) || (Equal::almostEqual(a1.x(), b1.x()) && Equal::almostEqual(a1.y(), b1.y())) ) {
                // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                int b0in = A.pointInPolygon(b0);
                int b2in = A.pointInPolygon(b2);
                if ( (b0in==true&&b2in==false)||(b0in==false&&b2in==true) ) {
                    return true;
                } else {
                    continue;
                }
            }

            if ( CSegment(a1,a2).onSegment(b2) || (Equal::almostEqual(a2.x(), b2.x()) && Equal::almostEqual(a2.y(), b2.y())) ) {
                // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                int b1in = A.pointInPolygon(b1);
                int b3in = A.pointInPolygon(b3);

                if ( (b1in==true && b3in==false)||(b1in==false&&b3in==true) ) {
                    return true;
                } else{ continue; }
            }

            if ( CSegment(b1,b2).onSegment(a1) || (Equal::almostEqual(a1.x(), b2.x()) && Equal::almostEqual(a1.y(), b2.y())) ) {
                // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                int a0in = B.pointInPolygon(a0);
                int a2in = B.pointInPolygon(a2);

                if ( (a0in==true&&a2in==false)||(a0in==false&&a2in==true) ) {
                    return true;
                } else { continue; }
            }

            if ( CSegment(b1,b2).onSegment(a2) || (Equal::almostEqual(a2.x(), b1.x()) && Equal::almostEqual(a2.y(), b1.y())) ) {
                // if a point is on a segment, it could intersect or it could not. Check via the neighboring points
                int a1in = B.pointInPolygon(a1);
                int a3in = B.pointInPolygon(a3);

                if ( (a1in==true&&a3in==false)||(a1in==false&&a3in==true) ) {
                    return true;
                } else{ continue; }
            }

            CPoint p = CSegment(b1, b2).intersect(CSegment(a1, a2), false);

            if( !p.isEmpty() ) { return true; }
        }
    }

    return false;
}

/**
* @brief Находится внутри другого
* @param other
* @return
*/
bool CPolygon::inside(const CPolygon &other) const
{
    if ( intersect(other) ) { return false; }
    return (other.pointInPolygon(points()[0]));
}

/**
* @brief Проецируем точки other к нашему по заданному направлению и отдаём расстояние
* @param other
* @return
*/
double CPolygon::projectionDistance(const CPolygon &other, const CVector2D &direction) const
{
    CPolygon edgeA(*this);
    CPolygon edgeB(other);

    edgeA.close();
    edgeB.close();

    CPoint offsetB = edgeB.offset;
    CPoint offsetA = edgeA.offset;

    double distance = NAN_DOUBLE;
    double d = NAN_DOUBLE;
    CPoint p, s1, s2;

    for(int i=0; i<edgeB.pointsCount(); ++i){
        //-- the shortest/most negative projection of B onto A
        double minProjection = NAN_DOUBLE;
        CPoint minP;
        for(int j=0; j<edgeA.pointsCount()-1; ++j) {
            p.set(edgeB[i].x() + offsetB.x(), edgeB[i].y() + offsetB.y());
            s1.set(edgeA[j].x() + offsetA.x(), edgeA[j].y() + offsetA.y());
            s2.set(edgeA[j+1].x() + offsetA.x(), edgeA[j+1].y() + offsetA.y());

            if ( qAbs((s2.y()-s1.y()) * direction.x() - (s2.x()-s1.x()) * direction.y()) < Equal::EPS ) { continue; }

            //-- project point, ignore edge boundaries
            d = CSegment(s1,s2).pointDistance(p, direction, false);

            if ( d==d && (minProjection!=minProjection || d<minProjection) ) {
                minProjection = d;
                minP = p;
            }
        }

        if ( (minProjection==minProjection) && ((distance!=distance)||(minProjection>distance)) ) {
            distance = minProjection;
        }
    }

    return distance;
}

/**
* @brief SlideDistance
* @param other
* @param direction
* @param ignoreNegative
* @return
*/
double CPolygon::slideDistance(const CPolygon &other, const CVector2D &direction, bool ignoreNegative) const
{
    CPolygon edgeA(*this);
    CPolygon edgeB(other);

    edgeA.close();
    edgeB.close();

    CPoint offsetB = edgeB.offset;
    CPoint offsetA = edgeA.offset;

    double distance = NAN_DOUBLE;

    CVector2D dir = direction;
    dir.normalize();

    for (int i=0; i<edgeB.pointsCount()-1; ++i) {
        for (int j=0; j<edgeA.pointsCount()-1; ++j) {
            CPoint A1(edgeA[j].x() + offsetA.x(), edgeA[j].y() + offsetA.y());
            CPoint A2(edgeA[j+1].x() + offsetA.x(), edgeA[j+1].y() + offsetA.y());
            CPoint B1(edgeB[i].x() + offsetB.x(), edgeB[i].y() + offsetB.y());
            CPoint B2(edgeB[i+1].x() + offsetB.x(), edgeB[i+1].y() + offsetB.y());

            if ( A1.isEq(A2) || B1.isEq(B2) ) { //-- Игнорим слишком короткие
                continue;
            }

            double d = CSegment(A1, A1).segmentDistance(CSegment(B1, B2), dir);

            if ( d==d && (distance!=distance || d<distance) ) {
                if ( !ignoreNegative || d>0 || Equal::almostEqual(d, 0) ) {
                    distance = d;
                }
            }
        }
    }

    return distance;
}


/**
* @brief Находим центр масс
* @return
*/
CPoint CPolygon::massCenter() const
{
    CPoint mc;
    double allS = 0;

    //-- По центрам масс треугольников
    for(int i=2; i<_points.count(); ++i) {
        const CPoint &p1 = _points[0];
        const CPoint &p2 = _points[i-1];
        const CPoint &p3 = _points[i];

        //-- Центр масс треугольника на пересечении медиан
        double cx = (p1.x()+p2.x()+p3.x())/3;
        double cy = (p1.y()+p2.y()+p3.y())/3;

        //-- Ориентированная площадь треугольника
        double v1x = p2.x() - p1.x();
        double v1y = p2.y() - p1.y();
        double v2x = p3.x() - p1.x();
        double v2y = p3.y() - p1.y();
        double s = 0.5*(v1x * v2y - v1y * v2x);

        mc.add(cx*s, cy*s);
        allS += s;
    }

    if ( Equal::almostEqual(allS, 0) ) return CPoint();

    mc.divide(allS);

    return mc;
}

bool CPolygon::isEq(const CPolygon &other) const
{
    CPoints p1 = points();
    CPoints p2 = other.points();

    if ( p1.count()!=p2.count() ) { return false; }

    std::sort(p1.begin(), p1.end());
    std::sort(p2.begin(), p2.end());

    for (int i=0; i<p1.count(); ++i) {
        if ( p1[i]!=p2[i] ) { return false; }
    }

    return true;
}

bool CPolygon::operator==(const CPolygon &other) const
{
    return isEq(other);
}
