#ifndef CMATRIX_H
#define CMATRIX_H

#include <QHash>
#include <QSize>

/**
* @brief Матрица и действия над ней
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
    CMatrix clon();
    CMatrix apply(const CMatrix & m) const;

    bool isDefault() const;

    friend QDebug operator<<(QDebug dbg, const CMatrix & m);

private:
    int _mi; //-- размер матрицы, строки
    int _mj; //-- размер матрицы, столбцы
    TMatrix _matrix; //-- сама матрица (одномерный массив, что бы не париться с вложенными)
};



#endif // CMATRIX_H
