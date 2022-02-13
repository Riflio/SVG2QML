#include "crect.h"
#include "cpath.h"
#include "cellipse.h"
#include "cline.h"

CRect::CRect(const CPoint& tl, const CPoint& br): CPrimitive(PT_RECT, tl, br)
{

}

void CRect::setX(double x)
{
    _points[0].setX(x);
}

void CRect::setY(double y)
{
    _points[0].setY(y);
}

void CRect::setWidth(double width)
{
    _points[1].setX(_points[0].x()+width);
}

void CRect::setHeight(double height)
{
    _points[1].setY(_points[0].y()+height);
}

void CRect::setRX(double x)
{
    _radius.first = x;
}

void CRect::setRY(double y)
{
    _radius.second = y;
}

CPoint CRect::topLeft() const
{
    return _points[0];
}

CPoint CRect::bottomRight() const
{
    return _points[1];
}

CSize CRect::size() const
{
    return CSize(_points[0], _points[1]);
}

CRect::TRadius CRect::radius() const
{
    return _radius;
}

/**
* @brief Преобразуем в path
* @return
*/
bool CRect::toPath()
{
    CPoint tl = _points[0];
    CSize s = size();

    double x = tl.x();
    double y = tl.y();
    double w = s.width();
    double h = s.height();
    double rx = _radius.first;
    double ry = _radius.second;

    CPath * p = new CPath();
    if ( Equal::almostEqual(rx, 0) && Equal::almostEqual(ry, 0) ) { //-- Если без скруглений, то тупо 4 линии
        CLine * lt = new CLine(CPoint(x, y), CPoint(x+w, y));
        CLine * lr = new CLine(CPoint(x+w, y), CPoint(x+w, y+h));
        CLine * lb = new CLine(CPoint(x+w, y+h), CPoint(x, y+h));
        CLine * ll = new CLine(CPoint(x, y+h), CPoint(x, y));

        p->addNext(lt);
        p->addNext(lr);
        p->addNext(lb);
        p->addNext(ll);
    } else { //-- Придётся делать скругления через Безье. Скругление возьмём от Элипса
        CLine * lt = new CLine(CPoint(x+rx, y), CPoint(x+w-rx, y));
        CBezier * btr = CEllipse::drawBezierEllipseQuarter(CPoint(x+w-rx, y+ry), QSizeF(-rx, ry));
        CLine * lr = new CLine(CPoint(x+w, y+ry), CPoint(x+w, y+h-ry));
        CBezier * bbr = CEllipse::drawBezierEllipseQuarter(CPoint(x+w-rx, y+h-ry), QSizeF(-rx, -ry));
        CLine * lb = new CLine(CPoint(x+w-rx, y+h), CPoint(x+rx, y+h));
        CBezier * bbl = CEllipse::drawBezierEllipseQuarter(CPoint(x+rx, y+h-ry), QSizeF(rx, -ry));
        CLine * ll = new CLine(CPoint(x, y+h-ry), CPoint(x, y+ry));
        CBezier * btl = CEllipse::drawBezierEllipseQuarter(CPoint(x+rx, y+ry), QSizeF(rx, ry));

        btr->reverse();
        bbl->reverse();

        p->addNext(lt);
        p->addNext(btr);
        p->addNext(lr);
        p->addNext(bbr);
        p->addNext(lb);
        p->addNext(bbl);
        p->addNext(ll);
        p->addNext(btl);
    }

    addNext(p);
    return true;
}
