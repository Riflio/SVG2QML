#ifndef CRECT_H
#define CRECT_H

#include "../Assets/cprimitive.h"

class CRect: public CPrimitive
{
public:
    CRect();
    typedef QPair<double, double> TRadius;

    void setX(double x);
    void setY(double y);
    void setWidth(double width);
    void setHeight(double height);
    void setRX(double x);
    void setRY(double y);

    CPoint topLeft() const;
    QSizeF size() const;

    TRadius radius() const;

    bool toPath() override;

private:
    CPoint _tl;
    QSizeF _size;
    TRadius _radius;
};

#endif // CRECT_H
