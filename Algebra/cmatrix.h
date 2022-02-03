#ifndef CMATRIX_H
#define CMATRIX_H

#include <QHash>
#include <QSize>

/**
* @brief The Matrix class
*/
class CMatrix {
public:
    CMatrix(int mi=3, int mj=3);
    CMatrix(int mi, int mj, const QList<double> &matrix);

    typedef QHash<int, double> TMatrix;

    enum SETBY { //-- для перевода из другого типа матриц в нашу
        SET_BY_ROWS,
        SET_BY_COLS
    };

    QSize size() const;
    TMatrix matrix() const;
    double getAt(int i, int j) const;

    CMatrix & set(int mi, int mj, const TMatrix & matrix, SETBY dir = SET_BY_ROWS);
    CMatrix & set(int mi, int mj, const QList<double> & matrix, SETBY dir = SET_BY_ROWS);
    CMatrix & setAt(int i, int j, double val);
    CMatrix & translate(double tx, double ty);
    CMatrix & scale(double sx, double sy);
    CMatrix & rotate(double angle);
    CMatrix & addition(const CMatrix & m);
    CMatrix & subtraction(const CMatrix & m);
    CMatrix & clear();

    CMatrix & multiplication(const CMatrix & m);
    CMatrix & multiplication(double n);
    CMatrix clon() const;
    CMatrix apply(const CMatrix & m) const;

    bool isDefault() const;

    friend QDebug operator<<(QDebug dbg, const CMatrix & m);

private:
    int _mi; //-- Rows count
    int _mj; //-- Cols count
    TMatrix _matrix; //-- Matrix data (one-dimensional array)
};



#endif // CMATRIX_H
