#ifndef FLINEARGRADIENT_H
#define FLINEARGRADIENT_H

#include <QPointF>
#include "../Assets/fgradient.h"

class FLinearGradient : public FGradient
{
public:
    FLinearGradient();
    FLinearGradient(const FLinearGradient &other);

    void setX1(float x);
    void setY1(float y);
    void setX2(float x);
    void setY2(float y);

    void setStartPoint(float x, float y);
    void setEndPoint(float x, float y);

    QPointF startPoint() const;
    QPointF endPoint() const;

    TDefType defType() const override { return DF_LINEARGRADIENT;  }

private:
    QPointF _startPoint;
    QPointF _endPoint;

};

#endif // FLINEARGRADIENT_H
