#ifndef CLINE_H
#define CLINE_H

#include "SVG/Assets/cprimitive.h"

class CLine: public CPrimitive<CLine>
{
public:
    CLine(CPoint s, CPoint e);
    CLine(const CLine &other);
    void cstream(QDataStream &dataStream, double scale) override;
    CPoints lianirize(double tol) const override;
    bool toPath() override;
};

#endif // CLINE_H
