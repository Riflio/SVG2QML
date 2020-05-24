#ifndef CBEZIER_H
#define CBEZIER_H

#include "SVG/Assets/cprimitive.h"

class CBezier : public CPrimitive
{
public:

    CBezier();
    CBezier(CPoint s, CPoint a1, CPoint a2, CPoint e);
    CBezier(const CBezier&other);
    CBezier(const CPoints &points);

    void cpaint(QPainter *painter, const CBoundingBox &area);
    void cstream(QDataStream &dataStream, double scale);

    bool isFlat(double tol) const;
    CPoints lianirize(double tol) const;

    const CBoundingBox &getBBox() const;

    QPair<CBezier,CBezier> subDivide(double t) const;

    QList<CBezier*> makeOffset(double d);


    struct TOffset {
        CPoint c;
        CPoint n;
        CPoint e;
    };

    TOffset boffset(double t, double d);
    QList<CBezier*> boffset(double t);

    CBezier *bscale(double d);

    bool direction();

    CPoint get(double t);

    CPoint normal(double t);
    CPoint derivative(double t);

    QList<CBezier *> reduce();

    struct TExtrema {
        QMap<int, QList<double>> result;
        QList<double> roots;
    };

    TExtrema bextrema();

    CPoint lli4(const CPoint &p1, const CPoint &p2, const CPoint &p3, CPoint const &p4) const;

    QList<CPoints> _dpoints;

    QList<double> droots(const QList<double> &p);

    CPoints hull(double t) const;

    CPoint lerp(double r, const CPoint &v1, const CPoint &v2) const;

    QList<CBezier *> split(double t1, double t2 = -1) const;

    double _t1 = 0;
    double _t2 = 1;

    double map(double v, double ds, double de, double ts, double te) const;

    bool simple();

    double angle(CPoint o, CPoint v1, CPoint v2) const;

    QList<CPoints> derive(const CPoints &points) const;

protected:    
    inline double evalBez(const QVector<double> poly, double t) const;

private:

};

#endif // CBEZIER_H
