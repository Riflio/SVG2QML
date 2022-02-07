#ifndef CMATRIX_H
#define CMATRIX_H

#include "cempty.h"
#include <QVector>
#include <QSize>

/**
* @brief The Matrix class
*/
class CMatrix: public CEmpty
{
public:
    typedef QVector<double> TMatrix;

    enum SETBY { //-- для перевода из другого типа матриц в нашу
        SET_BY_ROWS,
        SET_BY_COLS
    };

    CMatrix(const CMatrix &m);

    static CMatrix zeros(int cols, int rows);
    static CMatrix identity(int cols, int rows);
    static CMatrix initializer(int cols, int rows, const TMatrix l);

    QSize size() const;
    int rows() const;
    int cols() const;

    TMatrix data() const;
    CMatrix & setData(const TMatrix & matrix);
    CMatrix & setBy(int cols, int rows, const TMatrix &matrix, SETBY dir=SET_BY_ROWS);

    TMatrix row(int r) const;
    TMatrix col(int c) const;

    double getAt(int col, int row) const;
    CMatrix & setAt(int col, int row, double val);

    CMatrix & translate(double tx, double ty);
    CMatrix & scale(double sx, double sy);
    CMatrix & rotate(double angle);
    CMatrix & rotateD(double angle);

    CMatrix & addition(const CMatrix & m);
    CMatrix & subtraction(const CMatrix & m);    
    CMatrix & multiplication(const CMatrix & m);
    CMatrix & multiplication(double n);
    CMatrix & inverse();

    CMatrix & toIdentity();
    CMatrix & toZeros();

    CMatrix cofactor(int p, int q) const;
    CMatrix adjoint() const;

    double determinant() const;

    CMatrix clon() const;
    CMatrix apply(const CMatrix & m) const;

    bool isIdentity() const;
    bool isZeros() const;

    double& operator[] (int idx);
    double operator[] (int idx) const;

    friend QDebug operator<<(QDebug dbg, const CMatrix & m);

    bool operator==(const CEmptyPriv &) const;

private:
    CMatrix(int cols, int rows);
    CMatrix(int cols, int rows, const TMatrix &m);

    int _cols;
    int _rows;
    TMatrix _matrix; //-- Matrix data (one-dimensional array)
};



#endif // CMATRIX_H
