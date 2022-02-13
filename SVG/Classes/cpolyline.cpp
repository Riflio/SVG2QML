#include "cpolyline.h"
#include "cpath.h"
#include "cline.h"

CPolyline::CPolyline(): CPrimitive(PT_POLYLINE)
{

}

void CPolyline::addPoint(const CPoint &p)
{
    _points.add(p);
}

bool CPolyline::toPath()
{
    if ( _points.count()==0 ) return false;

    CPath * path = new CPath();

    CPoint prevPoint = _points[0];
    for(int i=1; i<_points.count(); ++i) {
        CLine * line = new CLine(prevPoint, _points[i]);
        path->addNext(line);
        prevPoint = _points[i];
    }

    addNext(path);
    return true;
}
