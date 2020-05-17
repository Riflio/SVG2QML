#ifndef CVECTOR2D_H
#define CVECTOR2D_H

#include <math.h>
#include "equal.h"
#include "cpoint.h"
/*
* Кэп, подсказывает, что это вектор
*/
class CVector2D
{
public:
    CVector2D();
    CVector2D(const CPoint & p);
    CVector2D(double x, double y);

    double x() const;
    double y() const;
    CPoint point() const;

    double length();
    double dot(const CVector2D & that);
    double cross(const CVector2D & that);

    CVector2D clon();
    CVector2D & divide(double scalar);
    CVector2D & unit();
    CVector2D & add(const CVector2D & that);
    CVector2D & subtract(const CVector2D & that);
    CVector2D & multiply(double scalar);

    CVector2D &perp();
    CVector2D & fromPoints(const CPoint & p1, const CPoint & p2);

    void normalize();

    friend QDebug operator<<(QDebug dbg, const CVector2D & v);

private:
    CPoint _p;

};

#endif // CVECTOR2D_H
