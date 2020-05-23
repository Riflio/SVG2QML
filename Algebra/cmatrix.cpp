#include "cmatrix.h"
#include "Algebra/equal.h"
#include "math.h"
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

CMatrix::CMatrix(int mi, int mj, const QList<double> &matrix)
    : _mi(mi), _mj(mj)
{
    set(mi, mj, matrix);
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

CMatrix &CMatrix::set(int mi, int mj, const QList<double> &matrix, CMatrix::SETBY dir)
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
* @param tx - сдвиг по X
* @param ty - сдвиг по Y
* @return
*/
CMatrix & CMatrix::translate(double tx, double ty)
{
    CMatrix mt(3, 3, {1, 0, tx, 0, 1, ty, 0, 0, 1});
    multiplication(mt);
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
    CMatrix ms(3, 3, {sx, 0, 0, 0, sy, 0, 0, 0, 1});
    multiplication(ms);
    return *this;
}

/**
* @brief Поворот
* @param angle - угол в градусах
* @return
*/
CMatrix &CMatrix::rotate(double angle)
{
    double a = angle *(M_PI/180);
    CMatrix mr(3, 3, {cos(a), -sin(a), 0, sin(a), cos(a), 0, 0, 0, 1});
    multiplication(mr);
    return *this;
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
    if ( _mj!=m._mi ) throw 21;

    TMatrix mn;
    mn.reserve(_mi*_mj);

    for (int i=0; i<_mi; i++) {
        for (int j=0; j< _mj; j++) {
            mn[i+j*_mi] = 0;
            for (int l=0; l< _mi; l++) {
                mn[i+j*_mi] += getAt(i, l) * m.getAt(l, j);
            }
        }
    }

    _matrix = mn;

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
    if ( _mj != m._mj ) throw 22;

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
             dbg.noquote()<<QString::number(m.getAt(i, j)).leftJustified(10, ' ');
        }
        dbg<<"\n";
    }
    return dbg.nospace();
}
