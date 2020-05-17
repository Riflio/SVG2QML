#include "cintersection.h"
#include "equal.h"

CIntersection::CIntersection()
{

}

CIntersection::CIntersectionResult CIntersection::intersectLinePoint(const CPoint & a1, const CPoint & a2,const CPoint & p)
{
    CIntersection::CIntersectionResult result;

    //qDebug()<<"intersectLinePoint"<<a1<<a2<<p;

    double l =  (p.x()-a1.x())*(a2.y()-a1.y());
    double r =  (p.y()-a1.y())*(a2.x()-a1.x());

    if (fabs (l-r)<p.epsilon()) {
        result.status=CIntersectionResult::S_INTERSECT;
        result.intersectionPoints.append(p);
    }

    return result;
}


CIntersection::CIntersectionResult CIntersection::intersectBezier3Line(const CPoint & p1, const CPoint & p2, const CPoint & p3, const CPoint & p4, const CPoint & a1, const CPoint & a2)
{
//    qDebug()<<"intersectBezier3Line"<<p1<<p2<<p3<<p4<<a1<<a2;

    CPoint a, b, c, d;
    CVector2D c3, c2, c1, c0;
    double cl;
    CVector2D n;    
    CPoint min = a1.min(a2);
    CPoint max = a1.max(a2);

    CIntersection::CIntersectionResult result;

    a=p1.clon().multiply(-1);
    b=p2.clon().multiply(3);
    c=p3.clon().multiply(-3);
    d=a.add(b).add(c).add(p4);
    c3=  CVector2D(d);

    a=p1.clon().multiply(3);
    b=p2.clon().multiply(-6);
    c=p3.clon().multiply(3);
    d=a.add(b).add(c);
    c2=  CVector2D(d);

    a=p1.clon().multiply(-3);
    b=p2.clon().multiply(3);
    c= a.add(b);
    c1= CVector2D(c);
    c0= CVector2D(p1);
    n= CVector2D(a1.y()-a2.y(), a2.x()-a1.x());
    cl=a1.x()*a2.y()-a2.x()*a1.y();

    CPolynomial::TCoefs roots = CPolynomial(n.dot(c3), n.dot(c2), n.dot(c1), n.dot(c0)+cl).getRoots();
    for(int i=0; i<roots.length(); i++){
        double t = roots.at(i);
        if( 0<=t&&t<=1 ){
            CPoint p5=p1.clon().lerp(p2,t);
            CPoint p6=p2.clon().lerp(p3,t);
            CPoint p7=p3.clon().lerp(p4,t);
            CPoint p8=p5.clon().lerp(p6,t);
            CPoint p9=p6.clon().lerp(p7,t);
            CPoint p10=p8.clon().lerp(p9,t);

       /*     if (intersectLinePoint(a1,a2, p10).status==CIntersectionResult::S_INTERSECT) {
                result.status=CIntersectionResult::S_INTERSECT;
                result.intersectionPoints.append(p10);
            }*/

            if( fabs(a1.x()-a2.x())<=Equal::EPS ){
                if( min.y()<=p10.y()&&p10.y()<=max.y() ){
                    result.status=CIntersectionResult::S_INTERSECT;
                    result.intersectionPoints.append(p10);
                }
            }else if( fabs(a1.y()-a2.y())<=Equal::EPS ){
                    if(min.x()<=p10.x()&&p10.x()<=max.x()){
                        result.status=CIntersectionResult::S_INTERSECT;
                        result.intersectionPoints.append(p10);
                    }
            }else if( p10.gte(min)&&p10.lte(max) ){
                result.status=CIntersectionResult::S_INTERSECT;
                result.intersectionPoints.append(p10);
            }
        }
    }   
    return result;
}


CIntersection::CIntersectionResult CIntersection::intersectLineLine( const CPoint & a1, const CPoint & a2,  const CPoint & b1, const CPoint & b2)
{   
    CIntersection::CIntersectionResult result;

    double ua_t = (b2.x()-b1.x())*(a1.y()-b1.y())-(b2.y()-b1.y())*(a1.x()-b1.x());
    double ub_t = (a2.x()-a1.x())*(a1.y()-b1.y())-(a2.y()-a1.y())*(a1.x()-b1.x());
    double u_b = (b2.y()-b1.y())*(a2.x()-a1.x())-(b2.x()-b1.x())*(a2.y()-a1.y());

    if( u_b!=0 ){
        double ua=ua_t/u_b;
        double ub=ub_t/u_b;
        if(0<=ua&&ua<=1&&0<=ub&&ub<=1){
            result.status = CIntersectionResult::S_INTERSECT;
            result.intersectionPoints.append(CPoint(a1.x()+ua*(a2.x()-a1.x()),a1.y()+ua*(a2.y()-a1.y())));
        } else {
            result.status = CIntersectionResult::S_NOINTERSECT;
        }
    } else {
        if(ua_t==0||ub_t==0){
            result.status = CIntersectionResult::S_COINCIDENT;
        } else {
            result.status = CIntersectionResult::S_PARALLEL;
        }
    }
    return result;
}



CIntersection::CIntersectionResult CIntersection::intersectBezier3Bezier3(const CPoint & a1, const CPoint & a2, const CPoint & a3, const CPoint & a4, const CPoint & b1, const CPoint & b2, const CPoint & b3, const CPoint & b4)
{
    CPoint a,b,c,d;
    CVector2D c13,c12,c11,c10;
    CVector2D c23,c22,c21,c20;
    CIntersection::CIntersectionResult result;

    a=a1.clon().multiply(-1);
    b=a2.clon().multiply(3);
    c=a3.clon().multiply(-3);
    d=a.add(b).add(c).add(a4);
    c13=CVector2D(d);
    a=a1.clon().multiply(3);
    b=a2.clon().multiply(-6);
    c=a3.clon().multiply(3);
    d=a.add(b).add(c);
    c12=CVector2D(d);
    a=a1.clon().multiply(-3);
    b=a2.clon().multiply(3);
    c=a.add(b);
    c11=CVector2D(c);
    c10=CVector2D(a1);
    a=b1.clon().multiply(-1);
    b=b2.clon().multiply(3);
    c=b3.clon().multiply(-3);
    d=a.add(b).add(c).add(b4);
    c23=CVector2D(d);
    a=b1.clon().multiply(3);
    b=b2.clon().multiply(-6);
    c=b3.clon().multiply(3);
    d=a.add(b).add(c);
    c22=CVector2D(d);
    a=b1.clon().multiply(-3);
    b=b2.clon().multiply(3);
    c=a.add(b);
    c21=CVector2D(c);
    c20=CVector2D(b1);
    double c10x2=c10.x()*c10.x();
    double c10x3=c10.x()*c10.x()*c10.x();
    double c10y2=c10.y()*c10.y();
    double c10y3=c10.y()*c10.y()*c10.y();
    double c11x2=c11.x()*c11.x();
    double c11x3=c11.x()*c11.x()*c11.x();
    double c11y2=c11.y()*c11.y();
    double c11y3=c11.y()*c11.y()*c11.y();
    double c12x2=c12.x()*c12.x();
    double c12x3=c12.x()*c12.x()*c12.x();
    double c12y2=c12.y()*c12.y();
    double c12y3=c12.y()*c12.y()*c12.y();
    double c13x2=c13.x()*c13.x();
    double c13x3=c13.x()*c13.x()*c13.x();
    double c13y2=c13.y()*c13.y();
    double c13y3=c13.y()*c13.y()*c13.y();
    double c20x2=c20.x()*c20.x();
    double c20x3=c20.x()*c20.x()*c20.x();
    double c20y2=c20.y()*c20.y();
    double c20y3=c20.y()*c20.y()*c20.y();
    double c21x2=c21.x()*c21.x();
    double c21x3=c21.x()*c21.x()*c21.x();
    double c21y2=c21.y()*c21.y();
    double c22x2=c22.x()*c22.x();
    double c22x3=c22.x()*c22.x()*c22.x();
    double c22y2=c22.y()*c22.y();
    double c23x2=c23.x()*c23.x();
    double c23x3=c23.x()*c23.x()*c23.x();
    double c23y2=c23.y()*c23.y();
    double c23y3=c23.y()*c23.y()*c23.y();

    CPolynomial poly(
        -c13x3*c23y3+c13y3*c23x3-3*c13.x()*c13y2*c23x2*c23.y()+3*c13x2*c13.y()*c23.x()*c23y2,
        -6*c13.x()*c22.x()*c13y2*c23.x()*c23.y()+6*c13x2*c13.y()*c22.y()*c23.x()*c23.y()+3*c22.x()*c13y3*c23x2-3*c13x3*c22.y()*c23y2-3*c13.x()*c13y2*c22.y()*c23x2+3*c13x2*c22.x()*c13.y()*c23y2,
        -6*c21.x()*c13.x()*c13y2*c23.x()*c23.y()-6*c13.x()*c22.x()*c13y2*c22.y()*c23.x()+6*c13x2*c22.x()*c13.y()*c22.y()*c23.y()+3*c21.x()*c13y3*c23x2+3*c22x2*c13y3*c23.x()+3*c21.x()*c13x2*c13.y()*c23y2-3*c13.x()*c21.y()*c13y2*c23x2-3*c13.x()*c22x2*c13y2*c23.y()+c13x2*c13.y()*c23.x()*(6*c21.y()*c23.y()+3*c22y2)+c13x3*(-c21.y()*c23y2-2*c22y2*c23.y()-c23.y()*(2*c21.y()*c23.y()+c22y2)),
        c11.x()*c12.y()*c13.x()*c13.y()*c23.x()*c23.y()-c11.y()*c12.x()*c13.x()*c13.y()*c23.x()*c23.y()+6*c21.x()*c22.x()*c13y3*c23.x()+3*c11.x()*c12.x()*c13.x()*c13.y()*c23y2+6*c10.x()*c13.x()*c13y2*c23.x()*c23.y()-3*c11.x()*c12.x()*c13y2*c23.x()*c23.y()-3*c11.y()*c12.y()*c13.x()*c13.y()*c23x2-6*c10.y()*c13x2*c13.y()*c23.x()*c23.y()-6*c20.x()*c13.x()*c13y2*c23.x()*c23.y()+3*c11.y()*c12.y()*c13x2*c23.x()*c23.y()-2*c12.x()*c12y2*c13.x()*c23.x()*c23.y()-6*c21.x()*c13.x()*c22.x()*c13y2*c23.y()-6*c21.x()*c13.x()*c13y2*c22.y()*c23.x()-6*c13.x()*c21.y()*c22.x()*c13y2*c23.x()+6*c21.x()*c13x2*c13.y()*c22.y()*c23.y()+2*c12x2*c12.y()*c13.y()*c23.x()*c23.y()+c22x3*c13y3-3*c10.x()*c13y3*c23x2+3*c10.y()*c13x3*c23y2+3*c20.x()*c13y3*c23x2+c12y3*c13.x()*c23x2-c12x3*c13.y()*c23y2-3*c10.x()*c13x2*c13.y()*c23y2+3*c10.y()*c13.x()*c13y2*c23x2-2*c11.x()*c12.y()*c13x2*c23y2+c11.x()*c12.y()*c13y2*c23x2-c11.y()*c12.x()*c13x2*c23y2+2*c11.y()*c12.x()*c13y2*c23x2+3*c20.x()*c13x2*c13.y()*c23y2-c12.x()*c12y2*c13.y()*c23x2-3*c20.y()*c13.x()*c13y2*c23x2+c12x2*c12.y()*c13.x()*c23y2-3*c13.x()*c22x2*c13y2*c22.y()+c13x2*c13.y()*c23.x()*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c13x2*c22.x()*c13.y()*(6*c21.y()*c23.y()+3*c22y2)+c13x3*(-2*c21.y()*c22.y()*c23.y()-c20.y()*c23y2-c22.y()*(2*c21.y()*c23.y()+c22y2)-c23.y()*(2*c20.y()*c23.y()+2*c21.y()*c22.y())),
        6*c11.x()*c12.x()*c13.x()*c13.y()*c22.y()*c23.y()+c11.x()*c12.y()*c13.x()*c22.x()*c13.y()*c23.y()+c11.x()*c12.y()*c13.x()*c13.y()*c22.y()*c23.x()-c11.y()*c12.x()*c13.x()*c22.x()*c13.y()*c23.y()-c11.y()*c12.x()*c13.x()*c13.y()*c22.y()*c23.x()-6*c11.y()*c12.y()*c13.x()*c22.x()*c13.y()*c23.x()-6*c10.x()*c22.x()*c13y3*c23.x()+6*c20.x()*c22.x()*c13y3*c23.x()+6*c10.y()*c13x3*c22.y()*c23.y()+2*c12y3*c13.x()*c22.x()*c23.x()-2*c12x3*c13.y()*c22.y()*c23.y()+6*c10.x()*c13.x()*c22.x()*c13y2*c23.y()+6*c10.x()*c13.x()*c13y2*c22.y()*c23.x()+6*c10.y()*c13.x()*c22.x()*c13y2*c23.x()-3*c11.x()*c12.x()*c22.x()*c13y2*c23.y()-3*c11.x()*c12.x()*c13y2*c22.y()*c23.x()+2*c11.x()*c12.y()*c22.x()*c13y2*c23.x()+4*c11.y()*c12.x()*c22.x()*c13y2*c23.x()-6*c10.x()*c13x2*c13.y()*c22.y()*c23.y()-6*c10.y()*c13x2*c22.x()*c13.y()*c23.y()-6*c10.y()*c13x2*c13.y()*c22.y()*c23.x()-4*c11.x()*c12.y()*c13x2*c22.y()*c23.y()-6*c20.x()*c13.x()*c22.x()*c13y2*c23.y()-6*c20.x()*c13.x()*c13y2*c22.y()*c23.x()-2*c11.y()*c12.x()*c13x2*c22.y()*c23.y()+3*c11.y()*c12.y()*c13x2*c22.x()*c23.y()+3*c11.y()*c12.y()*c13x2*c22.y()*c23.x()-2*c12.x()*c12y2*c13.x()*c22.x()*c23.y()-2*c12.x()*c12y2*c13.x()*c22.y()*c23.x()-2*c12.x()*c12y2*c22.x()*c13.y()*c23.x()-6*c20.y()*c13.x()*c22.x()*c13y2*c23.x()-6*c21.x()*c13.x()*c21.y()*c13y2*c23.x()-6*c21.x()*c13.x()*c22.x()*c13y2*c22.y()+6*c20.x()*c13x2*c13.y()*c22.y()*c23.y()+2*c12x2*c12.y()*c13.x()*c22.y()*c23.y()+2*c12x2*c12.y()*c22.x()*c13.y()*c23.y()+2*c12x2*c12.y()*c13.y()*c22.y()*c23.x()+3*c21.x()*c22x2*c13y3+3*c21x2*c13y3*c23.x()-3*c13.x()*c21.y()*c22x2*c13y2-3*c21x2*c13.x()*c13y2*c23.y()+c13x2*c22.x()*c13.y()*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c13x2*c13.y()*c23.x()*(6*c20.y()*c22.y()+3*c21y2)+c21.x()*c13x2*c13.y()*(6*c21.y()*c23.y()+3*c22y2)+c13x3*(-2*c20.y()*c22.y()*c23.y()-c23.y()*(2*c20.y()*c22.y()+c21y2)-c21.y()*(2*c21.y()*c23.y()+c22y2)-c22.y()*(2*c20.y()*c23.y()+2*c21.y()*c22.y())),
        c11.x()*c21.x()*c12.y()*c13.x()*c13.y()*c23.y()+c11.x()*c12.y()*c13.x()*c21.y()*c13.y()*c23.x()+c11.x()*c12.y()*c13.x()*c22.x()*c13.y()*c22.y()-c11.y()*c12.x()*c21.x()*c13.x()*c13.y()*c23.y()-c11.y()*c12.x()*c13.x()*c21.y()*c13.y()*c23.x()-c11.y()*c12.x()*c13.x()*c22.x()*c13.y()*c22.y()-6*c11.y()*c21.x()*c12.y()*c13.x()*c13.y()*c23.x()-6*c10.x()*c21.x()*c13y3*c23.x()+6*c20.x()*c21.x()*c13y3*c23.x()+2*c21.x()*c12y3*c13.x()*c23.x()+6*c10.x()*c21.x()*c13.x()*c13y2*c23.y()+6*c10.x()*c13.x()*c21.y()*c13y2*c23.x()+6*c10.x()*c13.x()*c22.x()*c13y2*c22.y()+6*c10.y()*c21.x()*c13.x()*c13y2*c23.x()-3*c11.x()*c12.x()*c21.x()*c13y2*c23.y()-3*c11.x()*c12.x()*c21.y()*c13y2*c23.x()-3*c11.x()*c12.x()*c22.x()*c13y2*c22.y()+2*c11.x()*c21.x()*c12.y()*c13y2*c23.x()+4*c11.y()*c12.x()*c21.x()*c13y2*c23.x()-6*c10.y()*c21.x()*c13x2*c13.y()*c23.y()-6*c10.y()*c13x2*c21.y()*c13.y()*c23.x()-6*c10.y()*c13x2*c22.x()*c13.y()*c22.y()-6*c20.x()*c21.x()*c13.x()*c13y2*c23.y()-6*c20.x()*c13.x()*c21.y()*c13y2*c23.x()-6*c20.x()*c13.x()*c22.x()*c13y2*c22.y()+3*c11.y()*c21.x()*c12.y()*c13x2*c23.y()-3*c11.y()*c12.y()*c13.x()*c22x2*c13.y()+3*c11.y()*c12.y()*c13x2*c21.y()*c23.x()+3*c11.y()*c12.y()*c13x2*c22.x()*c22.y()-2*c12.x()*c21.x()*c12y2*c13.x()*c23.y()-2*c12.x()*c21.x()*c12y2*c13.y()*c23.x()-2*c12.x()*c12y2*c13.x()*c21.y()*c23.x()-2*c12.x()*c12y2*c13.x()*c22.x()*c22.y()-6*c20.y()*c21.x()*c13.x()*c13y2*c23.x()-6*c21.x()*c13.x()*c21.y()*c22.x()*c13y2+6*c20.y()*c13x2*c21.y()*c13.y()*c23.x()+2*c12x2*c21.x()*c12.y()*c13.y()*c23.y()+2*c12x2*c12.y()*c21.y()*c13.y()*c23.x()+2*c12x2*c12.y()*c22.x()*c13.y()*c22.y()-3*c10.x()*c22x2*c13y3+3*c20.x()*c22x2*c13y3+3*c21x2*c22.x()*c13y3+c12y3*c13.x()*c22x2+3*c10.y()*c13.x()*c22x2*c13y2+c11.x()*c12.y()*c22x2*c13y2+2*c11.y()*c12.x()*c22x2*c13y2-c12.x()*c12y2*c22x2*c13.y()-3*c20.y()*c13.x()*c22x2*c13y2-3*c21x2*c13.x()*c13y2*c22.y()+c12x2*c12.y()*c13.x()*(2*c21.y()*c23.y()+c22y2)+c11.x()*c12.x()*c13.x()*c13.y()*(6*c21.y()*c23.y()+3*c22y2)+c21.x()*c13x2*c13.y()*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c12x3*c13.y()*(-2*c21.y()*c23.y()-c22y2)+c10.y()*c13x3*(6*c21.y()*c23.y()+3*c22y2)+c11.y()*c12.x()*c13x2*(-2*c21.y()*c23.y()-c22y2)+c11.x()*c12.y()*c13x2*(-4*c21.y()*c23.y()-2*c22y2)+c10.x()*c13x2*c13.y()*(-6*c21.y()*c23.y()-3*c22y2)+c13x2*c22.x()*c13.y()*(6*c20.y()*c22.y()+3*c21y2)+c20.x()*c13x2*c13.y()*(6*c21.y()*c23.y()+3*c22y2)+c13x3*(-2*c20.y()*c21.y()*c23.y()-c22.y()*(2*c20.y()*c22.y()+c21y2)-c20.y()*(2*c21.y()*c23.y()+c22y2)-c21.y()*(2*c20.y()*c23.y()+2*c21.y()*c22.y())),
        -c10.x()*c11.x()*c12.y()*c13.x()*c13.y()*c23.y()+c10.x()*c11.y()*c12.x()*c13.x()*c13.y()*c23.y()+6*c10.x()*c11.y()*c12.y()*c13.x()*c13.y()*c23.x()-6*c10.y()*c11.x()*c12.x()*c13.x()*c13.y()*c23.y()-c10.y()*c11.x()*c12.y()*c13.x()*c13.y()*c23.x()+c10.y()*c11.y()*c12.x()*c13.x()*c13.y()*c23.x()+c11.x()*c11.y()*c12.x()*c12.y()*c13.x()*c23.y()-c11.x()*c11.y()*c12.x()*c12.y()*c13.y()*c23.x()+c11.x()*c20.x()*c12.y()*c13.x()*c13.y()*c23.y()+c11.x()*c20.y()*c12.y()*c13.x()*c13.y()*c23.x()+c11.x()*c21.x()*c12.y()*c13.x()*c13.y()*c22.y()+c11.x()*c12.y()*c13.x()*c21.y()*c22.x()*c13.y()-c20.x()*c11.y()*c12.x()*c13.x()*c13.y()*c23.y()-6*c20.x()*c11.y()*c12.y()*c13.x()*c13.y()*c23.x()-c11.y()*c12.x()*c20.y()*c13.x()*c13.y()*c23.x()-c11.y()*c12.x()*c21.x()*c13.x()*c13.y()*c22.y()-c11.y()*c12.x()*c13.x()*c21.y()*c22.x()*c13.y()-6*c11.y()*c21.x()*c12.y()*c13.x()*c22.x()*c13.y()-6*c10.x()*c20.x()*c13y3*c23.x()-6*c10.x()*c21.x()*c22.x()*c13y3-2*c10.x()*c12y3*c13.x()*c23.x()+6*c20.x()*c21.x()*c22.x()*c13y3+2*c20.x()*c12y3*c13.x()*c23.x()+2*c21.x()*c12y3*c13.x()*c22.x()+2*c10.y()*c12x3*c13.y()*c23.y()-6*c10.x()*c10.y()*c13.x()*c13y2*c23.x()+3*c10.x()*c11.x()*c12.x()*c13y2*c23.y()-2*c10.x()*c11.x()*c12.y()*c13y2*c23.x()-4*c10.x()*c11.y()*c12.x()*c13y2*c23.x()+3*c10.y()*c11.x()*c12.x()*c13y2*c23.x()+6*c10.x()*c10.y()*c13x2*c13.y()*c23.y()+6*c10.x()*c20.x()*c13.x()*c13y2*c23.y()-3*c10.x()*c11.y()*c12.y()*c13x2*c23.y()+2*c10.x()*c12.x()*c12y2*c13.x()*c23.y()+2*c10.x()*c12.x()*c12y2*c13.y()*c23.x()+6*c10.x()*c20.y()*c13.x()*c13y2*c23.x()+6*c10.x()*c21.x()*c13.x()*c13y2*c22.y()+6*c10.x()*c13.x()*c21.y()*c22.x()*c13y2+4*c10.y()*c11.x()*c12.y()*c13x2*c23.y()+6*c10.y()*c20.x()*c13.x()*c13y2*c23.x()+2*c10.y()*c11.y()*c12.x()*c13x2*c23.y()-3*c10.y()*c11.y()*c12.y()*c13x2*c23.x()+2*c10.y()*c12.x()*c12y2*c13.x()*c23.x()+6*c10.y()*c21.x()*c13.x()*c22.x()*c13y2-3*c11.x()*c20.x()*c12.x()*c13y2*c23.y()+2*c11.x()*c20.x()*c12.y()*c13y2*c23.x()+c11.x()*c11.y()*c12y2*c13.x()*c23.x()-3*c11.x()*c12.x()*c20.y()*c13y2*c23.x()-3*c11.x()*c12.x()*c21.x()*c13y2*c22.y()-3*c11.x()*c12.x()*c21.y()*c22.x()*c13y2+2*c11.x()*c21.x()*c12.y()*c22.x()*c13y2+4*c20.x()*c11.y()*c12.x()*c13y2*c23.x()+4*c11.y()*c12.x()*c21.x()*c22.x()*c13y2-2*c10.x()*c12x2*c12.y()*c13.y()*c23.y()-6*c10.y()*c20.x()*c13x2*c13.y()*c23.y()-6*c10.y()*c20.y()*c13x2*c13.y()*c23.x()-6*c10.y()*c21.x()*c13x2*c13.y()*c22.y()-2*c10.y()*c12x2*c12.y()*c13.x()*c23.y()-2*c10.y()*c12x2*c12.y()*c13.y()*c23.x()-6*c10.y()*c13x2*c21.y()*c22.x()*c13.y()-c11.x()*c11.y()*c12x2*c13.y()*c23.y()-2*c11.x()*c11y2*c13.x()*c13.y()*c23.x()+3*c20.x()*c11.y()*c12.y()*c13x2*c23.y()-2*c20.x()*c12.x()*c12y2*c13.x()*c23.y()-2*c20.x()*c12.x()*c12y2*c13.y()*c23.x()-6*c20.x()*c20.y()*c13.x()*c13y2*c23.x()-6*c20.x()*c21.x()*c13.x()*c13y2*c22.y()-6*c20.x()*c13.x()*c21.y()*c22.x()*c13y2+3*c11.y()*c20.y()*c12.y()*c13x2*c23.x()+3*c11.y()*c21.x()*c12.y()*c13x2*c22.y()+3*c11.y()*c12.y()*c13x2*c21.y()*c22.x()-2*c12.x()*c20.y()*c12y2*c13.x()*c23.x()-2*c12.x()*c21.x()*c12y2*c13.x()*c22.y()-2*c12.x()*c21.x()*c12y2*c22.x()*c13.y()-2*c12.x()*c12y2*c13.x()*c21.y()*c22.x()-6*c20.y()*c21.x()*c13.x()*c22.x()*c13y2-c11y2*c12.x()*c12.y()*c13.x()*c23.x()+2*c20.x()*c12x2*c12.y()*c13.y()*c23.y()+6*c20.y()*c13x2*c21.y()*c22.x()*c13.y()+2*c11x2*c11.y()*c13.x()*c13.y()*c23.y()+c11x2*c12.x()*c12.y()*c13.y()*c23.y()+2*c12x2*c20.y()*c12.y()*c13.y()*c23.x()+2*c12x2*c21.x()*c12.y()*c13.y()*c22.y()+2*c12x2*c12.y()*c21.y()*c22.x()*c13.y()+c21x3*c13y3+3*c10x2*c13y3*c23.x()-3*c10y2*c13x3*c23.y()+3*c20x2*c13y3*c23.x()+c11y3*c13x2*c23.x()-c11x3*c13y2*c23.y()-c11.x()*c11y2*c13x2*c23.y()+c11x2*c11.y()*c13y2*c23.x()-3*c10x2*c13.x()*c13y2*c23.y()+3*c10y2*c13x2*c13.y()*c23.x()-c11x2*c12y2*c13.x()*c23.y()+c11y2*c12x2*c13.y()*c23.x()-3*c21x2*c13.x()*c21.y()*c13y2-3*c20x2*c13.x()*c13y2*c23.y()+3*c20y2*c13x2*c13.y()*c23.x()+c11.x()*c12.x()*c13.x()*c13.y()*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c12x3*c13.y()*(-2*c20.y()*c23.y()-2*c21.y()*c22.y())+c10.y()*c13x3*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c11.y()*c12.x()*c13x2*(-2*c20.y()*c23.y()-2*c21.y()*c22.y())+c12x2*c12.y()*c13.x()*(2*c20.y()*c23.y()+2*c21.y()*c22.y())+c11.x()*c12.y()*c13x2*(-4*c20.y()*c23.y()-4*c21.y()*c22.y())+c10.x()*c13x2*c13.y()*(-6*c20.y()*c23.y()-6*c21.y()*c22.y())+c20.x()*c13x2*c13.y()*(6*c20.y()*c23.y()+6*c21.y()*c22.y())+c21.x()*c13x2*c13.y()*(6*c20.y()*c22.y()+3*c21y2)+c13x3*(-2*c20.y()*c21.y()*c22.y()-c20y2*c23.y()-c21.y()*(2*c20.y()*c22.y()+c21y2)-c20.y()*(2*c20.y()*c23.y()+2*c21.y()*c22.y())),
        -c10.x()*c11.x()*c12.y()*c13.x()*c13.y()*c22.y()+c10.x()*c11.y()*c12.x()*c13.x()*c13.y()*c22.y()+6*c10.x()*c11.y()*c12.y()*c13.x()*c22.x()*c13.y()-6*c10.y()*c11.x()*c12.x()*c13.x()*c13.y()*c22.y()-c10.y()*c11.x()*c12.y()*c13.x()*c22.x()*c13.y()+c10.y()*c11.y()*c12.x()*c13.x()*c22.x()*c13.y()+c11.x()*c11.y()*c12.x()*c12.y()*c13.x()*c22.y()-c11.x()*c11.y()*c12.x()*c12.y()*c22.x()*c13.y()+c11.x()*c20.x()*c12.y()*c13.x()*c13.y()*c22.y()+c11.x()*c20.y()*c12.y()*c13.x()*c22.x()*c13.y()+c11.x()*c21.x()*c12.y()*c13.x()*c21.y()*c13.y()-c20.x()*c11.y()*c12.x()*c13.x()*c13.y()*c22.y()-6*c20.x()*c11.y()*c12.y()*c13.x()*c22.x()*c13.y()-c11.y()*c12.x()*c20.y()*c13.x()*c22.x()*c13.y()-c11.y()*c12.x()*c21.x()*c13.x()*c21.y()*c13.y()-6*c10.x()*c20.x()*c22.x()*c13y3-2*c10.x()*c12y3*c13.x()*c22.x()+2*c20.x()*c12y3*c13.x()*c22.x()+2*c10.y()*c12x3*c13.y()*c22.y()-6*c10.x()*c10.y()*c13.x()*c22.x()*c13y2+3*c10.x()*c11.x()*c12.x()*c13y2*c22.y()-2*c10.x()*c11.x()*c12.y()*c22.x()*c13y2-4*c10.x()*c11.y()*c12.x()*c22.x()*c13y2+3*c10.y()*c11.x()*c12.x()*c22.x()*c13y2+6*c10.x()*c10.y()*c13x2*c13.y()*c22.y()+6*c10.x()*c20.x()*c13.x()*c13y2*c22.y()-3*c10.x()*c11.y()*c12.y()*c13x2*c22.y()+2*c10.x()*c12.x()*c12y2*c13.x()*c22.y()+2*c10.x()*c12.x()*c12y2*c22.x()*c13.y()+6*c10.x()*c20.y()*c13.x()*c22.x()*c13y2+6*c10.x()*c21.x()*c13.x()*c21.y()*c13y2+4*c10.y()*c11.x()*c12.y()*c13x2*c22.y()+6*c10.y()*c20.x()*c13.x()*c22.x()*c13y2+2*c10.y()*c11.y()*c12.x()*c13x2*c22.y()-3*c10.y()*c11.y()*c12.y()*c13x2*c22.x()+2*c10.y()*c12.x()*c12y2*c13.x()*c22.x()-3*c11.x()*c20.x()*c12.x()*c13y2*c22.y()+2*c11.x()*c20.x()*c12.y()*c22.x()*c13y2+c11.x()*c11.y()*c12y2*c13.x()*c22.x()-3*c11.x()*c12.x()*c20.y()*c22.x()*c13y2-3*c11.x()*c12.x()*c21.x()*c21.y()*c13y2+4*c20.x()*c11.y()*c12.x()*c22.x()*c13y2-2*c10.x()*c12x2*c12.y()*c13.y()*c22.y()-6*c10.y()*c20.x()*c13x2*c13.y()*c22.y()-6*c10.y()*c20.y()*c13x2*c22.x()*c13.y()-6*c10.y()*c21.x()*c13x2*c21.y()*c13.y()-2*c10.y()*c12x2*c12.y()*c13.x()*c22.y()-2*c10.y()*c12x2*c12.y()*c22.x()*c13.y()-c11.x()*c11.y()*c12x2*c13.y()*c22.y()-2*c11.x()*c11y2*c13.x()*c22.x()*c13.y()+3*c20.x()*c11.y()*c12.y()*c13x2*c22.y()-2*c20.x()*c12.x()*c12y2*c13.x()*c22.y()-2*c20.x()*c12.x()*c12y2*c22.x()*c13.y()-6*c20.x()*c20.y()*c13.x()*c22.x()*c13y2-6*c20.x()*c21.x()*c13.x()*c21.y()*c13y2+3*c11.y()*c20.y()*c12.y()*c13x2*c22.x()+3*c11.y()*c21.x()*c12.y()*c13x2*c21.y()-2*c12.x()*c20.y()*c12y2*c13.x()*c22.x()-2*c12.x()*c21.x()*c12y2*c13.x()*c21.y()-c11y2*c12.x()*c12.y()*c13.x()*c22.x()+2*c20.x()*c12x2*c12.y()*c13.y()*c22.y()-3*c11.y()*c21x2*c12.y()*c13.x()*c13.y()+6*c20.y()*c21.x()*c13x2*c21.y()*c13.y()+2*c11x2*c11.y()*c13.x()*c13.y()*c22.y()+c11x2*c12.x()*c12.y()*c13.y()*c22.y()+2*c12x2*c20.y()*c12.y()*c22.x()*c13.y()+2*c12x2*c21.x()*c12.y()*c21.y()*c13.y()-3*c10.x()*c21x2*c13y3+3*c20.x()*c21x2*c13y3+3*c10x2*c22.x()*c13y3-3*c10y2*c13x3*c22.y()+3*c20x2*c22.x()*c13y3+c21x2*c12y3*c13.x()+c11y3*c13x2*c22.x()-c11x3*c13y2*c22.y()+3*c10.y()*c21x2*c13.x()*c13y2-c11.x()*c11y2*c13x2*c22.y()+c11.x()*c21x2*c12.y()*c13y2+2*c11.y()*c12.x()*c21x2*c13y2+c11x2*c11.y()*c22.x()*c13y2-c12.x()*c21x2*c12y2*c13.y()-3*c20.y()*c21x2*c13.x()*c13y2-3*c10x2*c13.x()*c13y2*c22.y()+3*c10y2*c13x2*c22.x()*c13.y()-c11x2*c12y2*c13.x()*c22.y()+c11y2*c12x2*c22.x()*c13.y()-3*c20x2*c13.x()*c13y2*c22.y()+3*c20y2*c13x2*c22.x()*c13.y()+c12x2*c12.y()*c13.x()*(2*c20.y()*c22.y()+c21y2)+c11.x()*c12.x()*c13.x()*c13.y()*(6*c20.y()*c22.y()+3*c21y2)+c12x3*c13.y()*(-2*c20.y()*c22.y()-c21y2)+c10.y()*c13x3*(6*c20.y()*c22.y()+3*c21y2)+c11.y()*c12.x()*c13x2*(-2*c20.y()*c22.y()-c21y2)+c11.x()*c12.y()*c13x2*(-4*c20.y()*c22.y()-2*c21y2)+c10.x()*c13x2*c13.y()*(-6*c20.y()*c22.y()-3*c21y2)+c20.x()*c13x2*c13.y()*(6*c20.y()*c22.y()+3*c21y2)+c13x3*(-2*c20.y()*c21y2-c20y2*c22.y()-c20.y()*(2*c20.y()*c22.y()+c21y2)),
        -c10.x()*c11.x()*c12.y()*c13.x()*c21.y()*c13.y()+c10.x()*c11.y()*c12.x()*c13.x()*c21.y()*c13.y()+6*c10.x()*c11.y()*c21.x()*c12.y()*c13.x()*c13.y()-6*c10.y()*c11.x()*c12.x()*c13.x()*c21.y()*c13.y()-c10.y()*c11.x()*c21.x()*c12.y()*c13.x()*c13.y()+c10.y()*c11.y()*c12.x()*c21.x()*c13.x()*c13.y()-c11.x()*c11.y()*c12.x()*c21.x()*c12.y()*c13.y()+c11.x()*c11.y()*c12.x()*c12.y()*c13.x()*c21.y()+c11.x()*c20.x()*c12.y()*c13.x()*c21.y()*c13.y()+6*c11.x()*c12.x()*c20.y()*c13.x()*c21.y()*c13.y()+c11.x()*c20.y()*c21.x()*c12.y()*c13.x()*c13.y()-c20.x()*c11.y()*c12.x()*c13.x()*c21.y()*c13.y()-6*c20.x()*c11.y()*c21.x()*c12.y()*c13.x()*c13.y()-c11.y()*c12.x()*c20.y()*c21.x()*c13.x()*c13.y()-6*c10.x()*c20.x()*c21.x()*c13y3-2*c10.x()*c21.x()*c12y3*c13.x()+6*c10.y()*c20.y()*c13x3*c21.y()+2*c20.x()*c21.x()*c12y3*c13.x()+2*c10.y()*c12x3*c21.y()*c13.y()-2*c12x3*c20.y()*c21.y()*c13.y()-6*c10.x()*c10.y()*c21.x()*c13.x()*c13y2+3*c10.x()*c11.x()*c12.x()*c21.y()*c13y2-2*c10.x()*c11.x()*c21.x()*c12.y()*c13y2-4*c10.x()*c11.y()*c12.x()*c21.x()*c13y2+3*c10.y()*c11.x()*c12.x()*c21.x()*c13y2+6*c10.x()*c10.y()*c13x2*c21.y()*c13.y()+6*c10.x()*c20.x()*c13.x()*c21.y()*c13y2-3*c10.x()*c11.y()*c12.y()*c13x2*c21.y()+2*c10.x()*c12.x()*c21.x()*c12y2*c13.y()+2*c10.x()*c12.x()*c12y2*c13.x()*c21.y()+6*c10.x()*c20.y()*c21.x()*c13.x()*c13y2+4*c10.y()*c11.x()*c12.y()*c13x2*c21.y()+6*c10.y()*c20.x()*c21.x()*c13.x()*c13y2+2*c10.y()*c11.y()*c12.x()*c13x2*c21.y()-3*c10.y()*c11.y()*c21.x()*c12.y()*c13x2+2*c10.y()*c12.x()*c21.x()*c12y2*c13.x()-3*c11.x()*c20.x()*c12.x()*c21.y()*c13y2+2*c11.x()*c20.x()*c21.x()*c12.y()*c13y2+c11.x()*c11.y()*c21.x()*c12y2*c13.x()-3*c11.x()*c12.x()*c20.y()*c21.x()*c13y2+4*c20.x()*c11.y()*c12.x()*c21.x()*c13y2-6*c10.x()*c20.y()*c13x2*c21.y()*c13.y()-2*c10.x()*c12x2*c12.y()*c21.y()*c13.y()-6*c10.y()*c20.x()*c13x2*c21.y()*c13.y()-6*c10.y()*c20.y()*c21.x()*c13x2*c13.y()-2*c10.y()*c12x2*c21.x()*c12.y()*c13.y()-2*c10.y()*c12x2*c12.y()*c13.x()*c21.y()-c11.x()*c11.y()*c12x2*c21.y()*c13.y()-4*c11.x()*c20.y()*c12.y()*c13x2*c21.y()-2*c11.x()*c11y2*c21.x()*c13.x()*c13.y()+3*c20.x()*c11.y()*c12.y()*c13x2*c21.y()-2*c20.x()*c12.x()*c21.x()*c12y2*c13.y()-2*c20.x()*c12.x()*c12y2*c13.x()*c21.y()-6*c20.x()*c20.y()*c21.x()*c13.x()*c13y2-2*c11.y()*c12.x()*c20.y()*c13x2*c21.y()+3*c11.y()*c20.y()*c21.x()*c12.y()*c13x2-2*c12.x()*c20.y()*c21.x()*c12y2*c13.x()-c11y2*c12.x()*c21.x()*c12.y()*c13.x()+6*c20.x()*c20.y()*c13x2*c21.y()*c13.y()+2*c20.x()*c12x2*c12.y()*c21.y()*c13.y()+2*c11x2*c11.y()*c13.x()*c21.y()*c13.y()+c11x2*c12.x()*c12.y()*c21.y()*c13.y()+2*c12x2*c20.y()*c21.x()*c12.y()*c13.y()+2*c12x2*c20.y()*c12.y()*c13.x()*c21.y()+3*c10x2*c21.x()*c13y3-3*c10y2*c13x3*c21.y()+3*c20x2*c21.x()*c13y3+c11y3*c21.x()*c13x2-c11x3*c21.y()*c13y2-3*c20y2*c13x3*c21.y()-c11.x()*c11y2*c13x2*c21.y()+c11x2*c11.y()*c21.x()*c13y2-3*c10x2*c13.x()*c21.y()*c13y2+3*c10y2*c21.x()*c13x2*c13.y()-c11x2*c12y2*c13.x()*c21.y()+c11y2*c12x2*c21.x()*c13.y()-3*c20x2*c13.x()*c21.y()*c13y2+3*c20y2*c21.x()*c13x2*c13.y(),
        c10.x()*c10.y()*c11.x()*c12.y()*c13.x()*c13.y()-c10.x()*c10.y()*c11.y()*c12.x()*c13.x()*c13.y()+c10.x()*c11.x()*c11.y()*c12.x()*c12.y()*c13.y()-c10.y()*c11.x()*c11.y()*c12.x()*c12.y()*c13.x()-c10.x()*c11.x()*c20.y()*c12.y()*c13.x()*c13.y()+6*c10.x()*c20.x()*c11.y()*c12.y()*c13.x()*c13.y()+c10.x()*c11.y()*c12.x()*c20.y()*c13.x()*c13.y()-c10.y()*c11.x()*c20.x()*c12.y()*c13.x()*c13.y()-6*c10.y()*c11.x()*c12.x()*c20.y()*c13.x()*c13.y()+c10.y()*c20.x()*c11.y()*c12.x()*c13.x()*c13.y()-c11.x()*c20.x()*c11.y()*c12.x()*c12.y()*c13.y()+c11.x()*c11.y()*c12.x()*c20.y()*c12.y()*c13.x()+c11.x()*c20.x()*c20.y()*c12.y()*c13.x()*c13.y()-c20.x()*c11.y()*c12.x()*c20.y()*c13.x()*c13.y()-2*c10.x()*c20.x()*c12y3*c13.x()+2*c10.y()*c12x3*c20.y()*c13.y()-3*c10.x()*c10.y()*c11.x()*c12.x()*c13y2-6*c10.x()*c10.y()*c20.x()*c13.x()*c13y2+3*c10.x()*c10.y()*c11.y()*c12.y()*c13x2-2*c10.x()*c10.y()*c12.x()*c12y2*c13.x()-2*c10.x()*c11.x()*c20.x()*c12.y()*c13y2-c10.x()*c11.x()*c11.y()*c12y2*c13.x()+3*c10.x()*c11.x()*c12.x()*c20.y()*c13y2-4*c10.x()*c20.x()*c11.y()*c12.x()*c13y2+3*c10.y()*c11.x()*c20.x()*c12.x()*c13y2+6*c10.x()*c10.y()*c20.y()*c13x2*c13.y()+2*c10.x()*c10.y()*c12x2*c12.y()*c13.y()+2*c10.x()*c11.x()*c11y2*c13.x()*c13.y()+2*c10.x()*c20.x()*c12.x()*c12y2*c13.y()+6*c10.x()*c20.x()*c20.y()*c13.x()*c13y2-3*c10.x()*c11.y()*c20.y()*c12.y()*c13x2+2*c10.x()*c12.x()*c20.y()*c12y2*c13.x()+c10.x()*c11y2*c12.x()*c12.y()*c13.x()+c10.y()*c11.x()*c11.y()*c12x2*c13.y()+4*c10.y()*c11.x()*c20.y()*c12.y()*c13x2-3*c10.y()*c20.x()*c11.y()*c12.y()*c13x2+2*c10.y()*c20.x()*c12.x()*c12y2*c13.x()+2*c10.y()*c11.y()*c12.x()*c20.y()*c13x2+c11.x()*c20.x()*c11.y()*c12y2*c13.x()-3*c11.x()*c20.x()*c12.x()*c20.y()*c13y2-2*c10.x()*c12x2*c20.y()*c12.y()*c13.y()-6*c10.y()*c20.x()*c20.y()*c13x2*c13.y()-2*c10.y()*c20.x()*c12x2*c12.y()*c13.y()-2*c10.y()*c11x2*c11.y()*c13.x()*c13.y()-c10.y()*c11x2*c12.x()*c12.y()*c13.y()-2*c10.y()*c12x2*c20.y()*c12.y()*c13.x()-2*c11.x()*c20.x()*c11y2*c13.x()*c13.y()-c11.x()*c11.y()*c12x2*c20.y()*c13.y()+3*c20.x()*c11.y()*c20.y()*c12.y()*c13x2-2*c20.x()*c12.x()*c20.y()*c12y2*c13.x()-c20.x()*c11y2*c12.x()*c12.y()*c13.x()+3*c10y2*c11.x()*c12.x()*c13.x()*c13.y()+3*c11.x()*c12.x()*c20y2*c13.x()*c13.y()+2*c20.x()*c12x2*c20.y()*c12.y()*c13.y()-3*c10x2*c11.y()*c12.y()*c13.x()*c13.y()+2*c11x2*c11.y()*c20.y()*c13.x()*c13.y()+c11x2*c12.x()*c20.y()*c12.y()*c13.y()-3*c20x2*c11.y()*c12.y()*c13.x()*c13.y()-c10x3*c13y3+c10y3*c13x3+c20x3*c13y3-c20y3*c13x3-3*c10.x()*c20x2*c13y3-c10.x()*c11y3*c13x2+3*c10x2*c20.x()*c13y3+c10.y()*c11x3*c13y2+3*c10.y()*c20y2*c13x3+c20.x()*c11y3*c13x2+c10x2*c12y3*c13.x()-3*c10y2*c20.y()*c13x3-c10y2*c12x3*c13.y()+c20x2*c12y3*c13.x()-c11x3*c20.y()*c13y2-c12x3*c20y2*c13.y()-c10.x()*c11x2*c11.y()*c13y2+c10.y()*c11.x()*c11y2*c13x2-3*c10.x()*c10y2*c13x2*c13.y()-c10.x()*c11y2*c12x2*c13.y()+c10.y()*c11x2*c12y2*c13.x()-c11.x()*c11y2*c20.y()*c13x2+3*c10x2*c10.y()*c13.x()*c13y2+c10x2*c11.x()*c12.y()*c13y2+2*c10x2*c11.y()*c12.x()*c13y2-2*c10y2*c11.x()*c12.y()*c13x2-c10y2*c11.y()*c12.x()*c13x2+c11x2*c20.x()*c11.y()*c13y2-3*c10.x()*c20y2*c13x2*c13.y()+3*c10.y()*c20x2*c13.x()*c13y2+c11.x()*c20x2*c12.y()*c13y2-2*c11.x()*c20y2*c12.y()*c13x2+c20.x()*c11y2*c12x2*c13.y()-c11.y()*c12.x()*c20y2*c13x2-c10x2*c12.x()*c12y2*c13.y()-3*c10x2*c20.y()*c13.x()*c13y2+3*c10y2*c20.x()*c13x2*c13.y()+c10y2*c12x2*c12.y()*c13.x()-c11x2*c20.y()*c12y2*c13.x()+2*c20x2*c11.y()*c12.x()*c13y2+3*c20.x()*c20y2*c13x2*c13.y()-c20x2*c12.x()*c12y2*c13.y()-3*c20x2*c20.y()*c13.x()*c13y2+c12x2*c20y2*c12.y()*c13.x()
    );
    CPolynomial::TCoefs roots=poly.getRootsInInterval(0,1);
    for(int i=0; i<roots.length(); i++){
        double s=roots.at(i);
        CPolynomial::TCoefs xRoots=CPolynomial(c13.x(),c12.x(),c11.x(),c10.x()-c20.x()-s*c21.x()-s*s*c22.x()-s*s*s*c23.x()).getRoots();
        CPolynomial::TCoefs yRoots=CPolynomial(c13.y(),c12.y(),c11.y(),c10.y()-c20.y()-s*c21.y()-s*s*c22.y()-s*s*s*c23.y()).getRoots();
        if(xRoots.length()>0&&yRoots.length()>0){
            double MTOLERANCE=1e-4;
            bool endCheckRoots = false;
            for(int j=0; j<xRoots.length(); j++) {
                if (endCheckRoots) break;
                double xRoot=xRoots.at(j);
                if( 0<=xRoot&&xRoot<=1 ) {
                    for(int k=0; k<yRoots.length(); k++) {
                        if(fabs (xRoot-yRoots.at(k))< MTOLERANCE){
                            result.intersectionPoints.append(c23.clon().multiply(s*s*s).add(c22.clon().multiply(s*s).add(c21.clon().multiply(s).add(c20))).point());
                            endCheckRoots = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    if(result.intersectionPoints.length()>0) {
        result.status=CIntersectionResult::S_INTERSECT;
    }

    return result;
}



















