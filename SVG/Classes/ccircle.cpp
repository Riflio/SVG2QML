#include "ccircle.h"

CCircle::CCircle()
    :CPrimitive(PT_CIRCLE)
{

}

CCircle::CCircle(const CCircle &other)
    :CPrimitive(other)
{
    _radius = other._radius;
}


CCircle::CCircle(CPoint center, double radius)
    :CPrimitive(PT_CIRCLE, center), _radius(radius)
{

}

void CCircle::setRadius(double radius)
{
    _radius = radius;
}

double CCircle::radius() const
{
    return _radius;
}

/**
* @brief Преобразуем в путь
* @return
*/
CPath *CCircle::toPath() const
{
/*
function drawBezierOvalQuarter(centerX, centerY, sizeX, sizeY) {
    ctx.beginPath();
    ctx.moveTo(
        centerX - (sizeX),
        centerY - (0)
    );
    ctx.bezierCurveTo(
        centerX - (sizeX),
        centerY - (0.552 * sizeY),
        centerX - (0.552 * sizeX),
        centerY - (sizeY),
        centerX - (0),
        centerY - (sizeY)
    );
        ctx.stroke();
}

function drawBezierOval(centerX, centerY, sizeX, sizeY) {
    drawBezierOvalQuarter(centerX, centerY, -sizeX, sizeY);
    drawBezierOvalQuarter(centerX, centerY, sizeX, sizeY);
    drawBezierOvalQuarter(centerX, centerY, sizeX, -sizeY);
    drawBezierOvalQuarter(centerX, centerY, -sizeX, -sizeY);
}

function drawBezierCircle(centerX, centerY, size) {
    drawBezierOval(centerX, centerY, size, size)
}

drawBezierCircle(200, 200, 64)

*/

}
