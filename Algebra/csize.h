#ifndef CSIZE_H
#define CSIZE_H

#include "equal.h"
#include "cpoint.h"
#include "cempty.h"

/**
* @brief The Size class
*/
class CSize: public CEmpty
{
public:
    CSize();
    CSize(double w, double h);
    CSize(const CPoint &tl, const CPoint &br);

    void setSize(double w, double h);

    double width() const;
    double height() const;

    void setWidth(double w);
    void setHeight(double h);

    bool isZero(double tolerance = Equal::EPS) const;

    bool operator==(const CEmptyPriv &) const override;

private:
    double _width;
    double _height;

};

#endif // CSIZE_H
