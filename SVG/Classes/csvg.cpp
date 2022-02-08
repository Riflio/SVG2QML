#include "csvg.h"

CSVG::CSVG():
    CPrimitive(PT_SVG)
{

}

QRectF CSVG::viewBox() const
{
    return _viewBox;
}

void CSVG::setViewBox(const QRectF& viewBox)
{
    _viewBox = viewBox;
}

CSize CSVG::size() const
{
    return _size;
}

void CSVG::setSize(const CSize& s)
{
    _size = s;
}

CPrimitive* CSVG::copy() const
{
    CPrimitive * prim = new CPrimitive(*this);
    CNodeInterface::reset(prim);
    return prim;
}
