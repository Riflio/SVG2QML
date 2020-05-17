#include "csegment.h"

CSegment::CSegment(const CPoint &a, const CPoint &b): _a(a), _b(b)
{

}

/**
* @brief Лежит ли точка p на отрезке, за исключением концов
* @param p
* @return
*/
bool CSegment::onSegment(const CPoint &p) const
{

    //-- Вертикальная линия
    if ( Equal::almostEqual(_a.x(), _b.x()) && Equal::almostEqual(p.x(), _a.x()) ){
        if( !Equal::almostEqual(p.y(), _b.y()) && !Equal::almostEqual(p.y(), _a.y()) && p.y() < qMax(_b.y(), _a.y()) && p.y() > qMin(_b.y(), _a.y()) ) {
            return true;
        } else{
            return false;
        }
    }

    //-- Горизонтальная
    if ( Equal::almostEqual(_a.y(), _b.y()) && Equal::almostEqual(p.y(), _a.y()) ){
        if( !Equal::almostEqual(p.x(), _b.x()) && !Equal::almostEqual(p.x(), _a.x()) && p.x() < qMax(_b.x(), _a.x()) && p.x() > qMin(_b.x(), _a.x()) ) {
            return true;
        } else {
            return false;
        }
    }

    //-- Проверим, что хотя бы лежит в пределах наших координат
    if ( (p.x() < _a.x() && p.x() < _b.x()) || (p.x() > _a.x() && p.x() > _b.x()) || (p.y() < _a.y() && p.y() < _b.y()) || (p.y() > _a.y() && p.y() > _b.y()) ) {
        return false;
    }

    //-- Убедимся, что ен на концах
    if ( p.isEq(_a) || p.isEq(_b) ){
        return false;
    }

    double cr = cross(p);

    if( qAbs(cr)>Equal::EPS ){
        return false;
    }

    double dt = dot(p);

    if( (dt<0) || Equal::almostEqual(dt, 0) ) {
        return false;
    }

    double l2 = len2();

    if( (dt>l2) || (Equal::almostEqual(dt, l2)) ) {
        return false;
    }

    return true;
}

double CSegment::cross(const CPoint &p) const
{
    return (p.y()-_a.y()) * (_b.x() - _a.x()) - (p.x() - _a.x()) * (_b.y() - _a.y());
}

double CSegment::dot(const CPoint &p) const
{
    return (p.x() - _a.x()) * (_b.x() - _a.x()) + (p.y() - _a.y())*(_b.y() - _a.y());
}

/**
* @brief Длина отрезка * 2
* @return
*/
double CSegment::len2() const
{
    return (_b.x() - _a.x())*(_b.x() - _a.x()) + (_b.y() - _a.y())*(_b.y() - _a.y());
}

/**
* @brief Точка пересечения
* @param other
* @param infinite - считаем, что бесконечной длины отрезки
* @return
*/
CPoint CSegment::intersect(const CSegment &other, bool infinite) const
{
    CPoint ip;
    double a1, a2, b1, b2, c1, c2, x, y;

    a1 = _b.y()-_a.y();
    b1 = _a.x()-_b.x();
    c1 = _b.x()*_a.y() - _a.x()*_b.y();
    a2 = other._b.y()-other._a.y();
    b2 = other._a.x()-other._b.x();
    c2 = other._b.x()*other._a.y() - other._a.x()*other._b.y();

    double denom = a1*b2 - a2*b1;

    x = (b1*c2 - b2*c1)/denom;
    y = (a2*c1 - a1*c2)/denom;

    if ( x!=x || y!=y ) { //-- NAN
        return ip;
    }


    if( !infinite ){
        //-- Совпадающие точки не считаются пересекающимися
        if (qAbs(_a.x()-_b.x()) > Equal::EPS && (( _a.x() < _b.x() ) ? x < _a.x() || x > _b.x() : x > _a.x() || x < _b.x() )) return ip;
        if (qAbs(_a.y()-_b.y()) > Equal::EPS && (( _a.y() < _b.y() ) ? y < _a.y() || y > _b.y() : y > _a.y() || y < _b.y() )) return ip;
        if (qAbs(other._a.x()-other._b.x()) > Equal::EPS && (( other._a.x() < other._b.x() ) ? x < other._a.x() || x > other._b.x() : x > other._a.x() || x < other._b.x() )) return ip;
        if (qAbs(other._a.y()-other._b.y()) > Equal::EPS && (( other._a.y() < other._b.y() ) ? y < other._a.y() || y > other._b.y() : y > other._a.y() || y < other._b.y() )) return ip;
    }

    ip.set(x, y);
    return ip;
}

/**
* @brief Расстояние до точки
* @param p
* @param normal
* @param infinite
* @return
*/
double CSegment::pointDistance(const CPoint &p, const CVector2D &normal, bool infinite) const
{
    CVector2D vNormal = normal;
    vNormal.normalize();
    CPoint dir(vNormal.y(), -vNormal.x());

    double pdot = p.x()*dir.x() + p.y()*dir.y();
    double s1dot = _a.x()*dir.x() + _a.y()*dir.y();
    double s2dot = _b.x()*dir.x() + _b.y()*dir.y();

    double pdotnorm = p.x()*vNormal.x() + p.y()*vNormal.y();
    double s1dotnorm = _a.x()*vNormal.x() + _a.y()*vNormal.y();
    double s2dotnorm = _b.x()*vNormal.x() + _b.y()*vNormal.y();

    if ( !infinite ) {
        if ( ((pdot<s1dot || Equal::almostEqual(pdot, s1dot)) && (pdot<s2dot || Equal::almostEqual(pdot, s2dot))) || ((pdot>s1dot || Equal::almostEqual(pdot, s1dot)) && (pdot>s2dot || Equal::almostEqual(pdot, s2dot))) ) {
            return NAN_DOUBLE; //-- dot doesn't collide with segment, or lies directly on the vertex
        }
        if ( (Equal::almostEqual(pdot, s1dot) && Equal::almostEqual(pdot, s2dot)) && (pdotnorm>s1dotnorm && pdotnorm>s2dotnorm) ) {
            return qMin(pdotnorm - s1dotnorm, pdotnorm - s2dotnorm);
        }
        if ( (Equal::almostEqual(pdot, s1dot) && Equal::almostEqual(pdot, s2dot)) && (pdotnorm<s1dotnorm && pdotnorm<s2dotnorm) ) {
            return -qMin(s1dotnorm-pdotnorm, s2dotnorm-pdotnorm);
        }
    }

    return -(pdotnorm - s1dotnorm + (s1dotnorm - s2dotnorm)*(s1dot - pdot)/(s1dot - s2dot));
}

/**
* @brief Расстояние до другого сегмента
* @param other
* @param normal
* @return
*/
double CSegment::segmentDistance(const CSegment &other, const CVector2D &direction) const
{

    CPoint normal(direction.y(), -direction.x());
    CVector2D reverse(-direction.x(), -direction.y());

    double dotA = _a.x()*normal.x() + _a.y()*normal.y();
    double dotB = _b.x()*normal.x() + _b.y()*normal.y();
    double dotE = other._a.x()*normal.x() + other._a.y()*normal.y();
    double dotF = other._b.x()*normal.x() + other._b.y()*normal.y();

    double crossA = _a.x()*direction.x() + _a.y()*direction.y();
    double crossB = _b.x()*direction.x() + _b.y()*direction.y();
    double crossE = other._a.x()*direction.x() + other._a.y()*direction.y();
    double crossF = other._b.x()*direction.x() + other._b.y()*direction.y();

    double ABmin = qMin(dotA, dotB);
    double ABmax = qMax(dotA, dotB);

    double EFmax = qMax(dotE, dotF);
    double EFmin = qMin(dotE, dotF);

    //-- segments that will merely touch at one point
    if (Equal::almostEqual(ABmax, EFmin) || Equal::almostEqual(ABmin, EFmax) ) {
        return NAN_DOUBLE;
    }

    //-- segments miss eachother completely
    if ( ABmax<EFmin || ABmin>EFmax ) {
        return NAN_DOUBLE;
    }

    double overlap = 0;

    if ( (ABmax>EFmax && ABmin<EFmin) || (EFmax>ABmax && EFmin<ABmin) ) {
        overlap = 1;
    } else {
        double minMax = qMin(ABmax, EFmax);
        double maxMin = qMax(ABmin, EFmin);

        double maxMax = qMax(ABmax, EFmax);
        double minMin = qMin(ABmin, EFmin);

        overlap = (minMax-maxMin)/(maxMax-minMin);
    }

    double crossABE = (other._a.y() - _a.y()) * (_b.x() - _a.x()) - (other._a.x() - _a.x()) * (_b.y() - _a.y());
    double crossABF = (other._b.y() - _a.y()) * (_b.x() - _a.x()) - (other._b.x() - _a.x()) * (_b.y() - _a.y());

    //-- lines are colinear
    if ( Equal::almostEqual(crossABE, 0) && Equal::almostEqual(crossABF, 0) ) {

        CPoint ABnorm(_b.y()-_a.y(), _a.x()-_b.x());
        CPoint EFnorm(other._b.y()-other._a.y(), other._a.x()-other._b.x());

        double ABnormlength = sqrt(ABnorm.x()*ABnorm.x() + ABnorm.y()*ABnorm.y());
        ABnorm.divide(ABnormlength);

        double EFnormlength = sqrt(EFnorm.x()*EFnorm.x() + EFnorm.y()*EFnorm.y());
        EFnorm.divide(EFnormlength);

        //-- segment normals must point in opposite directions
        if ( qAbs(ABnorm.y() * EFnorm.x() - ABnorm.x() * EFnorm.y()) < Equal::EPS && ABnorm.y() * EFnorm.y() + ABnorm.x() * EFnorm.x() < 0 ) {
            //-- normal of AB segment must point in same direction as given direction vector
            double normdot = ABnorm.y() * direction.y() + ABnorm.x() * direction.x();
            //-- the segments merely slide along eachother
            if ( Equal::almostEqual(normdot, 0) ) {
                return NAN_DOUBLE;
            }
            if ( normdot<0 ) {
                return 0;
            }
        }
        return NAN_DOUBLE;
    }

    QVector<double> distances;

    //-- coincident points
    if ( Equal::almostEqual(dotA, dotE) ) {
        distances.append(crossA-crossE);
    } else
    if ( Equal::almostEqual(dotA, dotF) ) {
        distances.append(crossA-crossF);
    } else
    if ( dotA>EFmin && dotA<EFmax ) {
        double d = CSegment(_a, other._a).pointDistance(other._b, reverse, false);
        if ( d==d && Equal::almostEqual(d, 0) ) { //--A currently touches EF, but AB is moving away from EF
            double dB = CSegment(_b, other._a).pointDistance(other._b, reverse, true);
            if ( dB<0 || Equal::almostEqual(dB*overlap, 0) ) {
                d = NAN_DOUBLE;
            }
        }
        if ( d==d ) {
            distances.append(d);
        }
    }

    if ( Equal::almostEqual(dotB, dotE) ) {
        distances.append(crossB-crossE);
    } else
    if ( Equal::almostEqual(dotB, dotF) ) {
        distances.append(crossB-crossF);
    } else
    if ( dotB>EFmin && dotB<EFmax ) {
        double d = CSegment(_b, other._a).pointDistance(other._b, reverse, false);

        if ( d==d && Equal::almostEqual(d, 0) ) { // crossA>crossB A currently touches EF, but AB is moving away from EF
            double dA = CSegment(_a, other._a).pointDistance(other._b, reverse, true);
            if( dA<0 || Equal::almostEqual(dA*overlap, 0) ) {
                d = NAN_DOUBLE;
            }
        }
        if ( d==d ) {
            distances.append(d);
        }
    }

    if ( dotE>ABmin && dotE<ABmax ) {
        double d = CSegment(other._a, _a).pointDistance(_b, direction, false);
        if ( d==d && Equal::almostEqual(d, 0) ) { //-- crossF<crossE A currently touches EF, but AB is moving away from EF
            double dF = CSegment(other._b, _a).pointDistance(_b, direction, true);
            if ( dF<0 || Equal::almostEqual(dF*overlap, 0) ) {
                d = NAN_DOUBLE;
            }
        }
        if ( d==d) {
            distances.append(d);
        }
    }

    if ( dotF>ABmin && dotF<ABmax ) {
        double d = CSegment(other._b, _a).pointDistance(_b, direction, false);
        if ( d==d && Equal::almostEqual(d, 0) ) { // && crossE<crossF A currently touches EF, but AB is moving away from EF
            double dE = CSegment(other._a, _a).pointDistance(_b, direction, true);
            if( dE<0 || Equal::almostEqual(dE*overlap, 0) ) {
                d = NAN_DOUBLE;
            }
        }

        if( d==d ) {
            distances.append(d);
        }
    }

    if ( distances.count()==0 ) {
        return NAN_DOUBLE;
    }

    return *std::min_element(distances.begin(), distances.end());
}
