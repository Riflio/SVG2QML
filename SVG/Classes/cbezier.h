#ifndef CBEZIER_H
#define CBEZIER_H

#include "SVG/Assets/cprimitive.h"

class CBezier : public CPrimitive
{
public:

    CBezier();
    CBezier(CPoint s, CPoint a1, CPoint a2, CPoint e);
    CBezier(const CBezier&other);

    void cpaint(QPainter *painter, const CBoundingBox &area);
    void cstream(QDataStream &dataStream, double scale);

    bool isFlat(double tol) const;
    CPoints lianirize(double tol) const;

    const CBoundingBox &getBBox() const;

    QPair<CBezier,CBezier> subDivide(double t) const;

    QList<CBezier*> makeOffset(double d);

protected:    
    inline double evalBez(const QVector<double> poly, double t) const;

private:

};

#endif // CBEZIER_H
