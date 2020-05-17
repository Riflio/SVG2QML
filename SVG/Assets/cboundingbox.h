#ifndef CBOUNDINGBOX_H
#define CBOUNDINGBOX_H

#include "Algebra/cpoint.h"

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
    QSizeF size() const;
    CPoint tl() const;
    CPoint br() const;

    void move(const CPoint &D);
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
    CPoint _tl; //-- Верхняя левая точка
    CPoint _br; //--Нижняя правая точка    
};

#endif // CBOUNDINGBOX_H
