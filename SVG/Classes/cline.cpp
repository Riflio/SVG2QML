#include "cline.h"
#include <QDebug>

/**
* @brief CLine
* @param s - точка начала
* @param e - точка конца
*/
CLine::CLine(CPoint s, CPoint e):
    CPrimitive(PT_LINE, s, e)
{

}

void CLine::cpaint(QPainter *painter, const CBoundingBox &area)
{
    QPainterPath path;

    path.moveTo(_points.p1().x(), _points.p1().y());
    path.lineTo(_points.p2().x(), _points.p2().y());

    drawPath(path, painter);
}


void CLine::cstream(QDataStream &dataStream, double scale)
{
    dataStream<<PT_LINE<<_points.p1()*scale<<_points.p2()*scale;
}

CPoints CLine::lianirize(double tol) const
{
    Q_UNUSED(tol);
    CPoints res;
    res.append(_points.p1());
    res.append(_points.p2());
    return res;
}
