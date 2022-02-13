#ifndef CSVG_H
#define CSVG_H

#include "../Assets/cprimitive.h"
#include "Algebra/csize.h"

#include <QRectF>

class CSVG: public CPrimitive<CSVG>
{
public:
    CSVG();

    QRectF viewBox() const;
    void setViewBox(const QRectF &viewBox);

    CSize size() const;
    void setSize(const CSize &s);

private:
    QRectF _viewBox;
    CSize _size;

};

#endif // CSVG_H
