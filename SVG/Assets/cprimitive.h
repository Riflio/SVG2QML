#ifndef CPRIMITIVE_H
#define CPRIMITIVE_H

#include <QPainter>
#include <QPainterPath>
#include <QList>

#include "cnode.h"
#include "Algebra/cpoint.h"
#include "cpoints.h"
#include "cboundingbox.h"

#include "CSS/style.h"


/**
* @brief Базовый класс с общими методами для всех отображаемых примитивов SVG (путь, линия, круг, группа и т.д)
*/
class CPrimitive: public CNodeInterface
{
public:
    enum PrimitiveType {
        PT_NONE,
        PT_BEZIER,
        PT_LINE,
        PT_PATH,
        PT_GROUP,
        PT_IMAGE,
        PT_POLYGON,
        PT_ARC,
        PT_RECT,
        PT_CIRCLE
    };

    CPrimitive(const CPrimitive&other);
    CPrimitive();
    CPrimitive(PrimitiveType type);

    virtual ~CPrimitive();

    CPrimitive(PrimitiveType type, const CPoint &p1);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5, const CPoint &p6);

    PrimitiveType type() const;
    CPoints points() const;
    void setPoints(const CPoints &points);
    int pointsCount() const;

    static const CBoundingBox allArea;

    virtual void cpaint(QPainter * painter, const CBoundingBox &area = CPrimitive::allArea);
    virtual void cstream(QDataStream &dataStream, double scale) { Q_UNUSED(dataStream); Q_UNUSED(scale); }

    virtual const CBoundingBox &getBBox() const;
    virtual void setStyles(CSS::Style styles);
    virtual CSS::Style styles() const;
    virtual void reverse();
    virtual CPoints lianirize(double tol) const;
    virtual void move(const CPoint &m);
    virtual void del();
    virtual CPrimitive * copy(bool nesteed=true) const;
    virtual void rotate(const CPoint &center, double angle);
    virtual void scale(double sX, double sY);

    virtual bool toPath();

    QString ID() const;
    void setID(QString id);

    QString classSVG() const;
    void setClassSVG(QString classSVG);

    friend QDataStream& operator <<(QDataStream &dataStream, const CPrimitive &p) { Q_UNUSED(p); return dataStream; }

    CPoint &operator[](int i);
    const CPoint &operator[](int i) const;

    virtual void setSelected(bool isSelected);

    double rotation;
    CPoint offset;
    bool marked;
    int source;
    bool flippedX;
    bool flippedY;

    void needUpdate();

protected:
    PrimitiveType _type;
    CPoints _points;
    CSS::Style _styles; //-- Собранные стили этого элемента
    mutable CBoundingBox _bbox; //-- Ограничительная рамка
    QString _id; //-- Айдишник из SVG
    QString _class; //-- Класс из SVG

    void drawPath(const QPainterPath &path, QPainter *painter);

};

#endif // CPRIMITIVE_H
