#include "cpolynomial.h"

CPolynomial::CPolynomial()
{

}


double CPolynomial::eval(double x)
{
    double result=0;
    for(int i=coefs.length()-1; i>=0; i--)
        result=result*x+coefs.at(i);
    return result;
}


int CPolynomial::getDegree() const
{
    return coefs.length()-1;
}

CPolynomial CPolynomial::multiply(const CPolynomial & that)
{
    CPolynomial result;
    int i,j;
    for(i=0; i<= getDegree()+that.getDegree(); i++)
        result.coefs.append(0);
    for(i=0; i<=getDegree(); i++)
        for(j=0; j<=that.getDegree(); j++)
            result.coefs[i+j]+= coefs.at(i)*that.coefs.at(j);
    return result;
}


CPolynomial & CPolynomial::divide_scalar(double scalar)
{
    for(int i=0; i<coefs.length(); i++)
        coefs[i] /=scalar;

    return *this;
}


CPolynomial & CPolynomial::simplify()
{
    for(int i=getDegree(); i>=0; i--){
        if(abs(coefs.at(i))<= TOLERANCE)
           ;//coefs.removeLast();
        else
            break;
    }
    return *this;
}


double CPolynomial::bisection(double min, double max)
{
    double minValue = eval(min);
    double maxValue = eval(max);
    double result = 0;
    if ( fabs(minValue)<=TOLERANCE )
        result=min;
    else if( fabs(maxValue)<=TOLERANCE )
        result=max;
    else if ( minValue*maxValue<=0 ) {
        double tmp1=log(max-min);
        double tmp2=log(10) * ACCURACY;
        double iters=ceil((tmp1+tmp2)/log(2));
        for (int i=0; i<iters; i++){
            result= 0.5*(min+max);
            double value= eval(result);
            if( fabs (value)<= TOLERANCE) { break; }
            if(value*minValue<0){
                max=result;
                maxValue=value;
            } else {
                min=result;
                minValue=value;
            }
        }
    }
    return result;
}



QString CPolynomial::toString()
{
    QStringList sCoefs;
    QStringList sSigns;

    for (int i=coefs.length()-1; i>=0; i--) {
        double value= coefs.at(i);
        QString sValue;
        if( qAbs(value)<TOLERANCE ){
            QString sign = (value<0)? " - " : " + ";
            value = fabs(value);
            if(i>0) sValue = ( qAbs(value-1)<TOLERANCE )? "x": sValue+"x";
            if( i>1 ) sValue+=QString("^%1").arg(i);

            sSigns.append(sign);
            sCoefs.append(sValue);
        }
    }

    sSigns[0] = (sSigns[0]==" + ")? "" : "-";

    QString result = "";
    for (int i=0;i < sCoefs.length(); i++) {
        result += sSigns.at(i)+sCoefs.at(i);
    }
    return result;
}



CPolynomial CPolynomial::getDerivative()
{
    CPolynomial derivative;
    for(int i=1; i<coefs.length(); i++) {
        derivative.coefs.append(i*coefs.at(i));
    }
    return derivative;
}


 CPolynomial::TCoefs CPolynomial::getRoots()
{
    TCoefs result;
    simplify();
    switch(getDegree()) {
        case 0:
        break;
        case 1:
            result= getLinearRoot();
        break;
        case 2:
            result= getQuadraticRoots();
        break;
        case 3:
            result= getCubicRoots();
        break;
        case 4:
            result= getQuarticRoots();
        break;
    }

    return result;
}

CPolynomial::TCoefs CPolynomial::getRootsInInterval(double min, double max)
{
    TCoefs roots;
    double root;
    if( getDegree()==1 ){
        root= bisection(min,max);
        if ( qAbs(root)<TOLERANCE )
            roots.append(root);
    } else {
        CPolynomial deriv= getDerivative();
        TCoefs droots= deriv.getRootsInInterval(min,max);
        if (droots.length()>0 ) {
            root= bisection(min,droots[0]);
            if ( qAbs(root)<TOLERANCE )
                roots.append(root);
            for (int i=0; i<=droots.length()-2; i++) {
                root= bisection(droots.at(i), droots.at(i+1));
                if( qAbs(root)<TOLERANCE )
                    roots.append(root);
            }
            root=bisection(droots.at(droots.length()-1), max);
            if ( qAbs(root)<TOLERANCE )
                roots.append(root);
        } else {
            root=bisection(min,max);
            if ( qAbs(root)<TOLERANCE ) roots.append(root);
        }
    }
    return roots;
}

CPolynomial::TCoefs CPolynomial::getLinearRoot()
{
    TCoefs result;
    double a= coefs.at(1);
    if( qAbs(a)<TOLERANCE ) result.append(-coefs.at(0)/a);
    return result;
}

CPolynomial::TCoefs CPolynomial::getQuadraticRoots()
{
    TCoefs results;
    if(getDegree()==2){
        double a=coefs.at(2);
        double b=coefs.at(1)/a;
        double c=coefs.at(0)/a;
        double d=b*b-4*c;
        if(d>0){
            double e= sqrt(d);
            results.append(0.5*(-b+e));
            results.append(0.5*(-b-e));
        } else if( qAbs(d)<TOLERANCE ) {
            results.append(0.5*-b);
        }
    }
    return results;
}

CPolynomial::TCoefs CPolynomial::getCubicRoots()
{
    TCoefs results;
    if(getDegree()==3) {
        double c3=coefs.at(3);
        double c2=coefs.at(2)/c3;
        double c1=coefs.at(1)/c3;
        double c0=coefs.at(0)/c3;
        double a=(3*c1-c2*c2)/3.0;
        double b=(2*c2*c2*c2-9*c1*c2+27*c0)/27.0;
        double offset=c2/3.0;
        double discrim=b*b/4 + a*a*a/27.0;
        double halfB=b/2.0;

        if (fabs(discrim)<TOLERANCE) {            
            discrim = 0;
        }
        if(discrim>0) {
            double e= sqrt(discrim);
            double tmp;
            double root;
            tmp=-halfB+e;
            if(tmp>=0)
                root= pow(tmp, 1/3.0);
            else
                root= -pow(-tmp,1/3.0);
            tmp=-halfB-e;
            if(tmp>=0)
                root+=pow(tmp,1/3.0);
            else
                root-=pow(-tmp,1/3.0);
            results.append(root-offset);
        } else if(discrim<0) {
            double distance= sqrt(-a/3.0);
            double angle= atan2( sqrt(-discrim),-halfB)/3.0;
            double cosa= cos(angle);
            double sina= sin(angle);
            double sqrt3 = sqrt(3.0);
            results.append(2*distance*cosa-offset);
            results.append(-distance*(cosa+sqrt3*sina)-offset);
            results.append(-distance*(cosa-sqrt3*sina)-offset);
        } else {
            double tmp;
            if(halfB>=0)
                tmp=-pow(halfB,1/3);
            else
                tmp=pow(-halfB,1/3);
            results.append(2*tmp-offset);
            results.append(-tmp-offset);
        }
    }
    return results;
}

CPolynomial::TCoefs CPolynomial::getQuarticRoots()
{
    TCoefs results;
    if(getDegree()==4){
        double c4=coefs.at(4);
        double c3=coefs.at(3)/c4;
        double c2=coefs.at(2)/c4;
        double c1=coefs.at(1)/c4;
        double c0=coefs.at(0)/c4;

        CPolynomial resolveRootsPolynominal(1,-c2,c3*c1-4*c0,-c3*c3*c0+4*c2*c0-c1*c1);
        TCoefs resolveRoots = resolveRootsPolynominal.getCubicRoots();

        double y=resolveRoots.at(0);
        double discrim=c3*c3/4-c2+y;
        if(fabs (discrim)<= TOLERANCE)
            discrim=0;
        if(discrim>0) {
            double e= sqrt(discrim);
            double t1=3*c3*c3/4-e*e-2*c2;
            double t2=(4*c3*c2-8*c1-c3*c3*c3)/(4*e);
            double plus=t1+t2;
            double minus=t1-t2;
            if(fabs (plus)<= TOLERANCE)
                plus=0;
            if(fabs (minus)<= TOLERANCE)
                minus=0;
            if(plus>=0){
                double f= sqrt(plus);
                results.append(-c3/4 + (e+f)/2);
                results.append(-c3/4 + (e-f)/2);
            }
            if(minus>=0) {
                double f= sqrt(minus);
                results.append(-c3/4 + (f-e)/2);
                results.append(-c3/4 - (f+e)/2);
            }
        } else if(discrim<0) {
        } else {
            double t2=y*y-4*c0;
            double t1=3*c3*c3/4-2*c2;
            if(t2>=-TOLERANCE) {
                if(t2<0)
                    t2=0;
                t2=2*sqrt(t2);
                if(t1+t2>= TOLERANCE) {
                    double d= sqrt(t1+t2);
                    results.append(-c3/4 + d/2);
                    results.append(-c3/4 - d/2);
                }
                if(t1-t2>= TOLERANCE) {
                    double d= sqrt(t1-t2);
                    results.append(-c3/4 + d/2);
                    results.append(-c3/4 - d/2);
                }
            }
        }
    }
    return results;
}
