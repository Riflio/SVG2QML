#include "cpoint.h"

CPoint::CPoint():
    marked(false), _x(0), _y(0), _epsilon(Equal::EPS), _empty(true)
{    
}

CPoint::CPoint(double x, double y, double epsilon):
    marked(false), _x(x), _y(y), _epsilon(epsilon), _empty(false)
{

}

CPoint::CPoint(const CPoint & p):
    marked(p.marked), _x(p._x), _y(p._y), _epsilon(p._epsilon), _empty(false)
{

}

CPoint::CPoint(const QPointF & p):
    marked(false), _x(p.x()), _y(p.y()), _epsilon(Equal::EPS), _empty(false)
{

}

double CPoint::x() const
{
    return _x;
}

double CPoint::y() const
{
    return _y;
}

double  CPoint::epsilon() const
{
    return _epsilon;
}

CPoint & CPoint::set(const CPoint & p2)
{
    _x = p2._x;
    _y = p2._y;
    _epsilon = p2._epsilon;
    _empty = false;
    return *this;
}

CPoint & CPoint::set(double x, double y)
{
    _x = x;
    _y = y;
    _empty = false;
    return *this;
}

CPoint &CPoint::setX(double x)
{
    _x = x;
    return *this;
}

CPoint &CPoint::setY(double y)
{
    _y = y;
    return *this;
}

CPoint &CPoint::incX(double x)
{
    _x += x;
    return *this;
}

CPoint &CPoint::incY(double y)
{
    _y += y;
    return *this;
}


CPoint & CPoint::subtract(const CPoint & p2)
{
    _x -= p2._x;
    _y -= p2._y;
    return *this;
}

CPoint & CPoint::multiply(double scalar)
{
    _x *= scalar;
    _y *= scalar;
    return *this;
}

CPoint & CPoint::divide(double scalar)
{
    _x /= scalar;
    _y /= scalar;
    return *this;
}

/**
* Расстояние до другой точки
* sw - маштаб по x
* sh - маштаб по y
*/
double CPoint::lengthTo(const CPoint & p2, double sw, double sh) const
{
    double dx= (p2._x - _x) / sw;
    double dy= (p2._y - _y) / sh;
    return sqrt(dx*dx+dy*dy);
}

CPoint & CPoint::add(const CPoint & p2)
{
    _x += p2._x;
    _y += p2._y;
    return *this;
}

CPoint & CPoint::add(double x, double y)
{
    _x += x;
    _y += y;
    return *this;
}


/**
* @brief Отражаем себя отсносительно себя
* @return
*/
CPoint & CPoint::reflect()
{
    _x = -_x;
    _y = -_y;
    return *this;
}

/**
* @brief Отражаем себя относительно другой точки
* @param p2
* @return
*/
CPoint & CPoint::reflectP(const CPoint & p2)
{
    double dx = (p2._x - _x);
    double dy = (p2._y - _y);

    _x = p2._x + dx;
    _y = p2._y + dy;

    return *this;
}

CPoint  CPoint::clon() const
{
    CPoint p2(_x, _y);
    return p2;
}

bool CPoint::isZero() const
{
    return ( (fabs (_x) < _epsilon) && (fabs (_y) < _epsilon) );
}

/**
* @brief Заданы или нет были координаты
* @return
*/
bool CPoint::isEmpty() const
{
    return _empty;
}

QString CPoint::toString() const
{
    return  QString("%1,%2").arg(x()).arg(y());
}

CPoint & CPoint::toZero()
{
    _x = 0.0;
    _y = 0.0;
    return *this;
}

bool CPoint::isEq(const CPoint &p2) const
{
    return ( Equal::almostEqual(_x, p2._x) && Equal::almostEqual(_y, p2._y) );
}

bool CPoint::isEq(double x, double y) const
{
    return ( Equal::almostEqual(_x, x) && Equal::almostEqual(_y, y) );
}

bool CPoint::lt(const CPoint & that) const
{
    return(_x<that._x&&_y<that._y);
}

bool CPoint::lte(const CPoint & that) const
{
    return(_x<=that._x&&_y<=that._y);
}

bool CPoint::gt(const CPoint & that) const
{
    return(_x>that._x&&_y>that._y);
}

bool CPoint::gte(const CPoint & that) const
{
    return(_x>=that._x&&_y>=that._y);
}

CPoint & CPoint::lerp(const CPoint & that, double t)
{
    _x = _x+(that._x-_x)*t;
    _y = _y+(that._y-_y)*t;

    return *this;
}

/**
* @brief Угол между точками p1 и p2 с центром у нас
* @param p1
* @param p2
* @return
*/
double CPoint::angle(const CPoint &p1, const CPoint &p2) const
{
    double dx1 = p1.x() - _x;
    double dy1 = p1.y() - _y;
    double dx2 = p2.x() - _x;
    double dy2 = p2.y() - _y;
    double cross = dx1*dy2 - dy1*dx2;
    double dot = dx1*dx2 + dy1*dy2;
    return atan2(cross, dot);
}

/**
* Применяем матрицу трансформации к точке
*/
CPoint & CPoint::transform(const CMatrix & m1)
{
    CMatrix m2(1, 3);
    m2.setAt(0,0,_x).setAt(0,1,_y).setAt(0,2, 1);
    CMatrix r = m1.apply(m2);
    set( r.getAt(0,0), r.getAt(1,0) );

    return *this;
}


CPoint CPoint::min(const CPoint & p2) const
{
    return CPoint(std::min(_x, p2._x), std::min(_y, p2._y));
}

CPoint CPoint::max(const CPoint & p2) const
{
    return CPoint(std::max(_x, p2._x), std::max(_y, p2._y));
}


QDebug operator<<(QDebug dbg, const CPoint & p)
{
    dbg<<"|"<<p._x<<p._y<<"|";
    return dbg.space();
}


CPoint CPoint::operator* (double v) const
{
    return CPoint(x()*v, y()*v, epsilon());
}

bool CPoint::operator ==(const CPoint &p) const
{
    return isEq(p);
}

bool CPoint::operator !=(const CPoint &p) const
{
    return !isEq(p);
}

CPoint::operator QPointF() const
{
    return QPointF(_x, _y);
}


QDataStream& operator <<(QDataStream &dataStream, const CPoint &p)
{
    dataStream<<p._x<<p._y;
    return dataStream;
}

bool operator<(const CPoint &p1, const CPoint &p2)
{
    return p1.lt(p2);
}

