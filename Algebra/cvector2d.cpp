#include "cvector2d.h"

CVector2D::CVector2D():
    _p(-100, 0)
{

}

CVector2D::CVector2D(const CPoint & p):
    _p(p)
{

}

CVector2D::CVector2D(double x, double y):
    _p(x,y)
{

}

double CVector2D::x() const
{
    return _p.x();
}

double CVector2D::y() const
{
    return _p.y();
}

CPoint  CVector2D::point() const
{
    return _p;
}

CVector2D  CVector2D::clon()
{
    return CVector2D(_p);
}

double CVector2D::length()
{
    return sqrt(_p.x()*_p.x()+_p.y()*_p.y());
}

double CVector2D::dot(const CVector2D & that)
{
    return _p.x()*that._p.x()+_p.y()*that._p.y();
}

double CVector2D::cross(const CVector2D & that)
{
    return _p.x()*that._p.y()-_p.y()*that._p.x();
}

CVector2D & CVector2D::divide(double scalar)
{    
    _p.divide(scalar);
    return *this;
}


CVector2D & CVector2D::unit()
{    
    return divide(length());
}


CVector2D & CVector2D::add(const CVector2D & that)
{
    _p.add(that._p);
    return *this;
}

CVector2D & CVector2D::subtract(const CVector2D & that)
{
    _p.set(_p.x() - that._p.x(),  _p.y() - that._p.y());
   return *this;
}


CVector2D & CVector2D:: multiply(double scalar)
{
    _p.set( _p.x() * scalar,  _p.y()*scalar );
    return *this;
}


CVector2D & CVector2D::perp()
{   
   _p.set(_p.x(), -_p.y());
   return *this;
}

CVector2D & CVector2D::fromPoints(const CPoint & p1, const CPoint & p2)
{
    _p.set( p2.x()- p1.x(), p2.y()- p1.y() );
    return *this;
}

/**
* @brief В единичный
*/
void CVector2D::normalize()
{
    _p.multiply(1.0/length());
}

QDebug operator<<(QDebug dbg, const CVector2D &v)
{
    dbg<<"|"<<v._p.x()<<v._p.y()<<"|";
    return dbg.space();
}






























