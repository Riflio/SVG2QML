#ifndef CPOLYNOMIAL_H
#define CPOLYNOMIAL_H

#include <math.h>
#include <QList>

#include <QDebug>

class CPolynomial
{
public:

    const double TOLERANCE =  1e-6;
    const double ACCURACY = 6;


    void set() {  }
    template<typename... Args>
    void set( double coef,  Args ... argss) {
        coefs.prepend(coef);
        return set(argss...);
    }

    template<typename... Args>
     CPolynomial(Args ... args) {
        set(args...);
     }
    CPolynomial();

    typedef QList<double> TCoefs;

    double eval(double x);
    int getDegree() const;
    double bisection(double min, double max);


    CPolynomial multiply(const CPolynomial & that);
    CPolynomial & divide_scalar(double scalar);
    CPolynomial & simplify();
    CPolynomial getDerivative();

    TCoefs getRoots();
    TCoefs getRootsInInterval(double min, double max);
    TCoefs getLinearRoot();
    TCoefs getQuadraticRoots();
    TCoefs getCubicRoots();
    TCoefs getQuarticRoots();

    QString toString();

    TCoefs coefs;
private:


};

#endif // CPOLYNOMIAL_H
