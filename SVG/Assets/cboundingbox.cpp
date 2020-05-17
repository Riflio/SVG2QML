#include "cboundingbox.h"
#include "SVG/Classes/cpolygon.h"

CBoundingBox::CBoundingBox()
{
    clear();
}

CBoundingBox::CBoundingBox(const QVector<CPoint> &points)
{
    clear();
    addPoints(points);
}

/**
* @brief Учитываем новую точку в размере ограничительнгой рамки
* @param p
* @return
*/
bool CBoundingBox::addPoint(const CPoint &p)
{
    if ( p.isEmpty() ) return false;

    _tl = _tl.min(p);
    _br = _br.max(p);

    return true;
}

/**
* @brief Добавляем новые точки, кэп
* @param points
*/
void CBoundingBox::addPoints(const QVector<CPoint> &points)
{
    foreach(const CPoint &p, points) {
        addPoint(p);
    }
}

/**
* @brief Учитываем другую ограничительную рамку
* @param b
* @return
*/
bool CBoundingBox::addBBox(const CBoundingBox &b)
{
    addPoint(b.tl());
    addPoint(b.br());
    return true;
}

/**
* @brief Обнуляем полностью
*/
void CBoundingBox::clear()
{
    _tl.set(MAX_DOUBLE, MAX_DOUBLE);
    _br.set(MIN_DOUBLE, MIN_DOUBLE);
}

/**
* @brief Отдаём размеры ограничительной рамки
* @return
*/
QSizeF CBoundingBox::size() const
{
    return QSizeF(width(), height());
}

/**
* @brief Верхний левый угол
* @return
*/
CPoint CBoundingBox::tl() const
{
    return _tl;
}

/**
* @brief Правый нижний
* @return
*/
CPoint CBoundingBox::br() const
{
    return _br;
}

/**
* @brief Перемещаем ограничительную рамку
* @param D - расстояние в приращениях
*/
void CBoundingBox::move(const CPoint &D)
{
    if ( isEmpty() ) return;
    _tl.add(D);
    _br.add(D);
}

/**
* @brief Область пустая?
* @return
*/
bool CBoundingBox::isEmpty() const
{
    return  ( _tl.isEq(_br) || (_tl.isEq(MAX_DOUBLE, MAX_DOUBLE) && _br.isEq(MIN_DOUBLE, MIN_DOUBLE)) );
}

/**
* @brief Проверяем, не находимся ли мы внутри другой ограничительной рамки
* @param other
* @return
*/
bool CBoundingBox::inside(const CBoundingBox &other) const
{
    CPolygon p1(tl(), br());
    CPolygon p2(other.tl(), other.br());
    return p1.inside(p2);
}

bool CBoundingBox::operator <(const CBoundingBox &other) const
{
    return ( (width()<other.width()) && (height()<other.height()));
}

bool CBoundingBox::operator >(const CBoundingBox &other) const
{
    return !(operator <(other));
}

void CBoundingBox::operator =(const QRectF &rect)
{
    if ( rect.isEmpty() ) {
        clear();
    } else {
        _tl.set(rect.left(), rect.top());
        _br.set(rect.right(), rect.bottom());
    }
}

double CBoundingBox::width() const
{
    return _br.x()-_tl.x();
}

double CBoundingBox::height() const
{
    return _br.y()-_tl.y();
}

double CBoundingBox::left() const
{
    return _tl.x();
}

double CBoundingBox::top() const
{
    return _tl.y();
}

CBoundingBox::operator QRectF() const
{
    return QRectF(_tl, _br);
}
