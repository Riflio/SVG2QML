#include "cmatrix.h"
#include "Algebra/equal.h"
#include <QDebug>

CMatrix::CMatrix(int mi, int mj):
    _mi(mi), _mj(mj)
{
    //-- Создаём базовую единичную матрицу
    for (int i=0; i<_mi; i++) {
        for (int j=0; j<_mj; j++) {
            setAt(i, j, (i==j)? 1: 0 );
        }
    }
}

/**
* @brief Отдаём размер нашей матрицы
* @return
*/
QSize CMatrix::size() const
{
    return QSize(_mi, _mj);
}


CMatrix::TMatrix CMatrix::matrix() const
{
    return _matrix;
}

/**
* @brief Устанавливаем значения в матрицу
* @param mi кол-во строк
* @param mj кол-во столбцов
* @param matrix
* @param dir направлени
* @return
*/
CMatrix & CMatrix::set(int mi, int mj, const TMatrix & matrix, SETBY dir)
{
    if ( matrix.count() != mi*mj ) throw 20;

    if ( dir == SET_BY_ROWS) {
        for (int i=0, d=0; i<mi; i++) {
            for (int j=0; j<mj; j++, d++) {
                setAt(i, j, matrix[d] );
            }
        }
    } else
    if( dir==SET_BY_COLS) {
        for (int i=0, d=0; i<mj; i++) {
            for (int j=0; j<mi; j++, d++) {
                setAt(j, i, matrix[d]);
            }
        }
    }

    return *this;
}


/**
* @brief Устанавливаем на конкретное место значение
* @param i
* @param j
* @param val
*/
CMatrix & CMatrix::setAt(int i, int j, double val)
{
    _matrix[i+j*_mi]=val;
    return *this;
}

/**
* @brief Отдаём значение по указанному месту
* @param i
* @param j
* @return
*/
double CMatrix::getAt(int i, int j) const
{
    return _matrix[i+j*_mi];
}

/**
* @brief Перемещение
* @param tx
* @param ty
* @return
*/
CMatrix & CMatrix::translate(double tx, double ty)
{
    setAt(0, _mj-1, tx);
    setAt(1, _mj-1, ty);

    return *this;
}

/**
* @brief Маштаб
* @param sx
* @param sy
* @return
*/
CMatrix & CMatrix::scale(double sx, double sy)
{
    setAt(0,0, sx);
    setAt(1,1, sy);

    return *this;
}

/**
* @brief Поворот
* @param angle
* @return
*/
CMatrix &CMatrix::rotate(double angle)
{

}

/**
* @brief Сложение матриц
* @param m
* @return
*/
CMatrix & CMatrix::addition(const CMatrix & m)
{
    if ( size() != m.size() ) throw 20;

    for (int i=0; i<_mj*_mi; i++) {
        _matrix[i] += m._matrix[i];
    }

    return *this;
}


/**
* @brief Вычитание матриц
* @param m
* @return
*/
CMatrix & CMatrix::subtraction(const CMatrix & m)
{
    if ( size() != m.size() ) throw 20;

    for (int i=0; i< _mj * _mi; i++) {
        _matrix[i] -= m._matrix[i];
    }

    return *this;
}

/**
* @brief Умножение на матрицу
* @param m
* @return
*/
CMatrix & CMatrix::multiplication(const CMatrix & m)
{
    if ( _mj!=m._mi ) throw 21; //--multiplication error: Target matrix count rows and source matrix count cols mismatch

     for (int i=0; i<_mi; i++) {
        for (int j=0; j< _mj; j++) {
            double summ = 0.0;
            for (int l=0; l< _mi; l++) {
                summ += getAt(i, l) * m.getAt(l, j);
            }

            setAt(i, j, summ);

        }
    }

     return *this;
}

/**
* @brief Домножаем каждый элемент на число
* @param n
* @return
*/
CMatrix &CMatrix::multiplication(double n)
{
    for (int i=0; i< _matrix.count(); i++) {
        _matrix[i] *= n;
    }
    return *this;
}

/**
* @brief Копируем матрицу
* @return
*/
CMatrix CMatrix::clon()
{
    CMatrix m2(_mi, _mj);

    for (int i=0; i< _matrix.count(); i++) {
        m2._matrix[i] = _matrix[i];
    }

    return m2;
}


/**
* @brief Применяем трансформации
* @param m
* @return
*/
CMatrix CMatrix::apply(const CMatrix & m) const
{
    if ( _mj != m._mj ) throw 22; //'apply error: Count cols target and source matrix mismatch');

    CMatrix r (m._mj, 1);

    for (int i=0; i < m._mj; i++) {
        double summ = 0.0;
        for (int j=0; j< _mj; j++) {
            summ += getAt(i, j)* m.getAt(0, j);
        }
        r.setAt(i, 0, summ);
    }

    return r;
}

/**
* @brief Преобразовываем в QMatrix
* @return
*/
QMatrix CMatrix::toQMatrix()
{
    QMatrix m;
    m.setMatrix(getAt(0,0), getAt(0,1), getAt(1,0), getAt(1,1), getAt(0,2), getAt(1,2));
    return m;
}

/**
* @brief Отвечаем, если матрица единичная
* @return
*/
bool CMatrix::isDefault() const
{
    for (int i=0; i<_mi; i++) {
        for (int j=0; j<_mj; j++) {
            if ( !Equal::almostEqual(_matrix[i+j*_mi], ((i==j)? 1: 0)) )  {
                 return false;
            }
        }
    }

    return true;
}


QDebug operator<<(QDebug dbg, const CMatrix & m)
{
    dbg<<"\n";

    for (int i=0;  i< m._mi; i++) {
        for (int j=0; j< m._mj; j++) {
             dbg<<"   "<<m.getAt(i, j);
        }
        dbg<<"\n";
    }

    return dbg.space();
}
