#ifndef FLINEARGRADIENT_H
#define FLINEARGRADIENT_H

#include <QPointF>
#include "../Assets/fgradient.h"

class FLinearGradient : public FGradient
{
public:
    FLinearGradient();
    FLinearGradient(const FLinearGradient &other);

    void setStartPoint(float x, float y);
    void setEndPoint(float x, float y);

    QPointF startPoint() const;
    QPointF endPoint() const;

private:
    QPointF _startPoint;
    QPointF _endPoint;

};

#endif // FLINEARGRADIENT_H
