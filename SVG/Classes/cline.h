#ifndef CLINE_H
#define CLINE_H

#include "SVG/Assets/cprimitive.h"

class CLine: public CPrimitive
{
public:
    CLine(CPoint s, CPoint e);
    void cpaint(QPainter *painter, const CBoundingBox &area);
    void cstream(QDataStream &dataStream, double scale);
    CPoints lianirize(double tol) const;

};

#endif // CLINE_H
