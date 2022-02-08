#ifndef FLINEARGRADIENT_H
#define FLINEARGRADIENT_H

#include "SVG/Assets/fgradient.h"
#include "Algebra/cpoint.h"

class FLinearGradient : public FGradient
{
public:
    FLinearGradient();
    FLinearGradient(const FLinearGradient &other);
    FLinearGradient(const FGradient &other);

    void setX1(float x);
    void setY1(float y);
    void setX2(float x);
    void setY2(float y);

    void setStartPoint(const CPoint &s);
    void setEndPoint(const CPoint &e);

    CPoint startPoint() const;
    CPoint &startPoint();
    CPoint endPoint() const;
    CPoint &endPoint();

    TDefType defType() const override { return DF_LINEARGRADIENT;  }

    CPrimitive* copy() const override;

private:
    CPoint _startPoint;
    CPoint _endPoint;

};

#endif // FLINEARGRADIENT_H
