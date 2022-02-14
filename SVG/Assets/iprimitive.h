#ifndef IPRIMITIVE_H
#define IPRIMITIVE_H

#include "cpoints.h"
#include "cboundingbox.h"
#include "CSS/style.h"
#include "cnode.h"

class IPrimitive: public virtual INodeInterface
{
public:
    enum PrimitiveType {
        PT_NONE,
        PT_SVG,
        PT_BEZIER,
        PT_LINE,
        PT_PATH,
        PT_GROUP,
        PT_IMAGE,
        PT_POLYGON,
        PT_ARC,
        PT_RECT,
        PT_CIRCLE,
        PT_ELLIPSE,
        PT_POLYLINE,
        PT_DEF
    };

    virtual  PrimitiveType type() const =0;
    virtual CPoints points() const =0;
    virtual void setPoints(const CPoints &points) =0;
    virtual int pointsCount() const =0;

    virtual void cstream(QDataStream &dataStream, double scale) =0;

    virtual CBoundingBox boundingBox(bool withTransform=true) const =0;

    virtual void setStyles(CSS::Style styles) =0;
    virtual CSS::Style styles() const =0;
    virtual void reverse() =0;
    virtual CPoints lianirize(double tol) const =0;
    virtual IPrimitive * clone() const =0;

    virtual bool applyTransform(const CMatrix &matrix) =0;

    virtual bool toPath() =0;

    virtual QString ID() const =0;
    virtual void setID(QString id) =0;

    virtual QString className() const =0;
    virtual void setClassName(QString className) =0;

    virtual void setTitle(QString title) =0;
    virtual QString title() const =0;

    virtual void setDescr(QString descr) =0;
    virtual QString descr() const =0;

    virtual CPoint &operator[](int i) =0;
    virtual const CPoint &operator[](int i) const =0;

    virtual void needUpdate() =0;

    virtual CMatrix transformation() const =0;
    virtual void setTransformation(const CMatrix &matrix) =0;
};

#endif // IPRIMITIVE_H
