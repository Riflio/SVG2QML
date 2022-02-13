#ifndef CBEZIER_H
#define CBEZIER_H

#include "SVG/Assets/cprimitive.h"

class CBezier : public CPrimitive<CBezier>
{
public:
    CBezier();
    CBezier(const CPoint &s, const CPoint &a1, const CPoint &a2, const CPoint &e);
    CBezier(const CPoint &s, const CPoint &a, const CPoint &e);
    CBezier(const CBezier&other);
    CBezier(const CPoints &points);

    void cstream(QDataStream &dataStream, double scale) override;

    CPoints lianirize(double tol) const override;

    CBoundingBox boundingBox(bool withTransform=true) const override;

    QPair<CBezier, CBezier> subDivide(double t) const;
    QList<CBezier *> split(double t1, double t2 = -1) const;

    QList<CBezier*> makeOffset(double d) const;

    bool direction();

    CPoint get(double t) const;
    QList<CBezier *> reduce() const;

    bool isFlat(double tol) const;
    bool simple() const;

    CBezier & operator =(const CBezier &b);

protected:    
    struct TExtrema {
        QMap<int, QList<double>> result;
        QList<double> roots;
    };
    struct TOffset {
        CPoint c;
        CPoint n;
        CPoint e;
    };
    CBezier * bscale(double d);
    CPoint normal(double t) const;
    CPoint derivative(double t) const;
    double evalBez(const QVector<double> poly, double t) const;
    QList<double> droots(const QList<double> &p) const;
    QList<CPoints> dPoints() const;
    QList<CPoints> derive(const CPoints &points) const;
    double map(double v, double ds, double de, double ts, double te) const;
    CPoint lerp(double r, const CPoint &v1, const CPoint &v2) const;
    CPoints hull(double t) const;
    TExtrema extrema() const;
    CPoint lli4(const CPoint &p1, const CPoint &p2, const CPoint &p3, CPoint const &p4) const;
    TOffset boffset(double t, double d);
    QList<CBezier*> boffset(double t) const;

private:
    double _t1 = 0;
    double _t2 = 1;
    mutable QList<CPoints> _dpoints;

};

#endif // CBEZIER_H
