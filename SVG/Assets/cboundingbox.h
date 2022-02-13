#ifndef CBOUNDINGBOX_H
#define CBOUNDINGBOX_H

#include "Algebra/cpoint.h"
#include "Algebra/csize.h"
#include "Algebra/cmatrix.h"

/**
* @brief Вычисляем размеры ограничительной рамки
*/
class CBoundingBox
{
public:
    CBoundingBox();
    CBoundingBox(const QVector<CPoint> &points);

    bool addPoint(const CPoint &p);
    void addPoints(const QVector<CPoint> &points);
    bool addBBox(const CBoundingBox &b);
    void clear();
    CSize size() const;
    CPoint tl() const;
    CPoint br() const;

    CBoundingBox & move(const CPoint &D);
    CBoundingBox & transform(const CMatrix &transformMatrix);
    bool isEmpty() const;

    bool inside(const CBoundingBox &other) const;

    bool operator <(const CBoundingBox &other) const;
    bool operator >(const CBoundingBox &other) const;
    void operator =(const QRectF &rect);

    operator QRectF() const;

    double width() const;
    double height() const;
    double left() const;
    double top() const;

private:
    CPoint _tl; //-- Top Left point
    CPoint _br; //-- Bottom Right point

};

#endif // CBOUNDINGBOX_H
