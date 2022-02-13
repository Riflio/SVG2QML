#ifndef CPRIMITIVE_H
#define CPRIMITIVE_H

#include <QPainter>
#include <QPainterPath>
#include <QList>

#include "iprimitive.h"

#include "Algebra/cpoint.h"
#include "Algebra/cmatrix.h"


/**
* @brief Базовый класс с общими методами для всех отображаемых примитивов SVG (путь, линия, круг, группа и т.д)
*/
template<class T>
class CPrimitive: public CNodeInterface, public virtual IPrimitive
{
public:
    CPrimitive(const CPrimitive&other);
    CPrimitive();
    CPrimitive(PrimitiveType type);

    virtual ~CPrimitive();

    CPrimitive(PrimitiveType type, const CPoint &p1);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4);
    CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5, const CPoint &p6);
    CPrimitive(PrimitiveType type, const CPoints &points);

    PrimitiveType type() const override;
    CPoints points() const override;
    void setPoints(const CPoints &points) override;
    int pointsCount() const override;

    virtual void cstream(QDataStream &dataStream, double scale) { Q_UNUSED(dataStream); Q_UNUSED(scale); }

    CBoundingBox boundingBox(bool withTransform=true) const override;

    void setStyles(CSS::Style styles) override;
    CSS::Style styles() const override;
    void reverse() override;
    CPoints lianirize(double tol) const override;
    CPrimitive * clone() const override;

    bool applyTransform(const CMatrix &matrix) override;

    bool toPath() override;

    QString ID() const override;
    void setID(QString id) override;

    QString className() const override;
    void setClassName(QString className) override;

    void setTitle(QString title) override;
    QString title() const override;

    void setDescr(QString descr) override;
    QString descr() const override;

    friend QDataStream& operator <<(QDataStream &dataStream, const CPrimitive &p) { Q_UNUSED(p); return dataStream; }

    CPoint &operator[](int i) override;
    const CPoint &operator[](int i) const override;

    void needUpdate() override;

    CMatrix transformation() const override;
    void setTransformation(const CMatrix &matrix) override;

protected:
    PrimitiveType _type;
    CPoints _points;
    CSS::Style _styles; //-- Собранные стили этого элемента
    mutable CBoundingBox _boundingBox; //-- Bounding Box
    QString _id; //-- ID from SVG
    QString _className; //-- Class name from SVG

    CMatrix _transformMatrix = CMatrix::identity(3,3);

    QString _title;
    QString _descr;

};
#include "cprimitive.cpp"

#endif // CPRIMITIVE_H
