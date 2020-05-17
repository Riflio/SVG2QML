#ifndef CINTERSECTION_H
#define CINTERSECTION_H

#include "cpoint.h"
#include "cvector2d.h"
#include "cpolynomial.h"

/**
* @brief Определяем пересечения прямых/безье и тд
*/
class CIntersection
{
public:
    CIntersection();

    class CIntersectionResult {
    public:
        enum STATUS {
            S_NOINTERSECT, //-- не пересекаются
            S_INTERSECT, //-- пересекаются
            S_COINCIDENT, //-- совпадают
            S_PARALLEL //--параллельны
        };

        STATUS status = S_NOINTERSECT;
        QList<CPoint> intersectionPoints;

        /**
         * @brief отдаём ту, у которой наименьшее расстояние до указанной
         * @param CPoint from
         * @return
         */
        CPoint minFromPoint;
        double minFromLength=std::numeric_limits<double>::max();
        bool updateMinFrom(const CPoint & from) {             
            if (status!=S_INTERSECT || intersectionPoints.length()==0) return false;

            CPoint p, result;
            double cl;
            for(int i=0; i<intersectionPoints.length(); i++) {
                p = intersectionPoints.at(i);
                cl = from.lengthTo(p);

                if (cl<minFromLength) {
                    minFromPoint = p;
                    minFromLength = cl;
                }
            }

            return true;
        }

    };

    CIntersectionResult intersectLinePoint(const CPoint & a1, const CPoint & a2,const CPoint & p);
    CIntersectionResult intersectLineLine( const CPoint & a1, const CPoint & a2,  const CPoint & b1, const CPoint & b2);
    CIntersectionResult intersectBezier3Line(const CPoint & p1, const CPoint & p2, const CPoint & p3, const CPoint & p4, const CPoint & a1, const CPoint & a2);
    CIntersectionResult intersectBezier3Bezier3(const CPoint & a1, const CPoint & a2, const CPoint & a3, const CPoint & a4, const CPoint & b1, const CPoint & b2, const CPoint & b3, const CPoint & b4);

private:

};

#endif // CINTERSECTION_H
