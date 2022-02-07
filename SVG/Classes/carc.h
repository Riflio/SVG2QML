#ifndef CARC_H
#define CARC_H

#include "../Assets/cprimitive.h"

#include "cbezier.h"
#include "cpath.h"

class CArc: public CPrimitive
{
public:
    CArc();
    CArc(CPoint startPoint, double rx, double ry, double rotation, bool largeArc, bool sweep, CPoint endPoint);

    CPrimitive* copy() const override;

    double rx() const;
    double ry() const;
    double rotation() const;
    bool largeArcFlag() const;
    bool sweepFlag() const;

    bool toPath() override;

    const double TAU = M_PI*2;

private:
    double _rx;
    double _ry;
    double _rotation;
    bool _largeArc;
    bool _sweep;

    struct TArcCenter {
        CPoint p;
        double ang1;
        double ang2;
    };

    TArcCenter getArcCenter(CPoint s, CPoint e, double rx, double ry, bool largeArc, bool sweep, double sinphi, double cosphi, double pxp, double pyp) const;
    QList<CPoint> approxUnitArc(double ang1, double ang2) const;
    CPoint mapToEllipse(CPoint p, double rx, double ry, double cosphi, double sinphi, CPoint cp) const;
};

#endif // CARC_H
