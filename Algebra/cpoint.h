#ifndef CPOINT_H
#define CPOINT_H

#include <math.h>
#include <QPoint>
#include <QDataStream>
#include <QDebug>

#include "equal.h"
#include "cmatrix.h"

/**
* @brief Точка и действия над ней
*/


class CPoint
{
public:
    CPoint(double x, double y, double epsilon=Equal::EPS);
    CPoint(const CPoint & p);
    CPoint(const QPointF & p);
    CPoint();

    double x() const;
    double y() const;
    double epsilon() const;

    CPoint & set(const CPoint & p2);
    CPoint & set(double x, double y);

    CPoint & setX(double x);
    CPoint & setY(double y);

    CPoint & add(const CPoint & p2);
    CPoint & add(double x, double y);
    CPoint & subtract(const CPoint & p2);
    CPoint & multiply(double scalar);
    CPoint & divide(double scalar);

    CPoint min(const CPoint & p2) const;
    CPoint max(const CPoint & p2) const;

    CPoint & reflect();
    CPoint & reflectP(const CPoint & p2);
    CPoint & toZero();
    CPoint & transform(const CMatrix & m1);
    CPoint & lerp(const CPoint & that, double t);

    //-- Сравнение
    bool lt(const CPoint & that) const;
    bool lte(const CPoint & that) const;
    bool gt(const CPoint & that) const;
    bool gte(const CPoint & that) const;
    bool isEq(const CPoint & p2) const;
    bool isEq(double x, double y) const;

    CPoint clon() const;
    double lengthTo(const CPoint & p2, double sw=1, double sh=1) const;
    bool isZero() const;
    bool isEmpty() const;

    QString toString() const;

    CPoint operator* (double v) const;

    friend QDebug operator<<(QDebug dbg, const CPoint & p);

    friend QDataStream& operator <<(QDataStream &dataStream, const CPoint &p);

    friend bool operator<(const CPoint &p1, const CPoint& p2);

    bool operator ==(const CPoint &p) const;
    bool operator !=(const CPoint &p) const;

    operator QPointF() const;

    bool marked;

private:
    double _x;
    double _y;
    double _epsilon; //-- Погрешность
    bool _empty;
};

inline uint qHash(const CPoint &key, uint seed) {
    return qHash(key.x(), seed) + qHash(key.y(), seed);
}

Q_DECLARE_METATYPE(CPoint*)

#endif // CPOINT_H
