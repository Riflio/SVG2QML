#ifndef CPOINTS_H
#define CPOINTS_H

#include "Algebra/cpoint.h"

/**
* @brief Обёртка над списком точек для упрощения частых действий
*/
class CPoints: public QVector<CPoint>
{
public:
    CPoints();
    CPoints(const QVector<CPoint> &points);

    const CPoint &p1() const;
    const CPoint &p2() const;
    const CPoint &p3() const;
    const CPoint &p4() const;
    const CPoint &p5() const;
    const CPoint &p6() const;

    void add(const CPoint &p);
    void reverse();
    void move(const CPoint &d);
    void rotate(const CPoint &center, double degrees);

private:


};

#endif // CPOINTS_H
