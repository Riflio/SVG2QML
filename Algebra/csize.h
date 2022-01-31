#ifndef CSIZE_H
#define CSIZE_H

#include "equal.h"

/**
* @brief The Size class
*/
class CSize
{
public:
    CSize();
    CSize(double w, double h);

    void setSize(double w, double h);

    double width() const;
    double height() const;

    void setWidth(double w);
    void setHeight(double h);

    bool isZero() const;

private:
    double _width;
    double _height;
    double _epsilon;

};

#endif // CSIZE_H
