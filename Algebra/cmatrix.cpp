#include "cmatrix.h"
#include "Algebra/equal.h"
#include "math.h"
#include <QDebug>

/**
* @brief CMatrix constructor
* @param cols count
* @param rows count
*/
CMatrix::CMatrix(int cols, int rows): _cols(cols), _rows(rows)
{
    _matrix.reserve(_cols*_rows);
}

CMatrix::CMatrix(int cols, int rows, const TMatrix& m): _cols(cols), _rows(rows), _matrix(m)
{

}

CMatrix::CMatrix(const CMatrix& m): _cols(m._cols), _rows(m._rows), _matrix(m._matrix)
{

}

/**
* @brief Construct with zeros data
* @param cols
* @param rows
* @return
*/
CMatrix CMatrix::zeros(int cols, int rows)
{
    CMatrix mz(cols, rows);
    mz.toZeros();
    return mz;
}

/**
* @brief Construct with identity matrix
* @param cols
* @param rows
* @return
*/
CMatrix CMatrix::identity(int cols, int rows)
{
    CMatrix md(cols, rows);
    md.toIdentity();
    return md;
}

/**
* @brief Construct from data
* @param cols
* @param rows
* @param l
* @return
*/
CMatrix CMatrix::initializer(int cols, int rows, const TMatrix l)
{
    CMatrix mi(cols, rows);
    mi.setData(l);
    return mi;
}

/**
* @brief Size of matrix
* @return
*/
QSize CMatrix::size() const
{
    return QSize(_cols, _rows);
}

/**
* @brief Rows count
* @return
*/
int CMatrix::rows() const
{
    return _rows;
}

/**
* @brief Cols count
* @return
*/
int CMatrix::cols() const
{
    return _cols;
}

/**
* @brief Data values as QMap
* @return
*/
CMatrix::TMatrix CMatrix::data() const
{
    return _matrix;
}

/**
* @brief Data of row
* @param r
* @return
*/
CMatrix::TMatrix CMatrix::row(int r) const
{
    TMatrix rl;
    for (int i=0; i<_cols; ++i) {
        rl<<_matrix[r*_cols+i];
    }
    return rl;
}

/**
* @brief data of column
* @param c
* @return
*/
CMatrix::TMatrix CMatrix::col(int c) const
{
    TMatrix cl;
    for (int i=0; i<_rows; ++i) {
        cl<<_matrix[i*_rows+c];
    }
    return cl;
}

/**
* @brief Set values to matrix
* @param matrix
* @return
*/
CMatrix & CMatrix::setData(const TMatrix & matrix)
{    
    _matrix = matrix;
    return *this;
}

/**
* @brief Set values to matrix
* @param cols
* @param rows
* @param matrix
* @param dir
* @return
*/
CMatrix &CMatrix::setBy(int cols, int rows, const QList<double> &matrix, CMatrix::SETBY dir)
{
    if ( matrix.count()>cols*rows ) { throw 20; }

    for (int i=0; i<matrix.count(); ++i) {
        if ( dir==SET_BY_ROWS ) { _matrix[(i%cols)+(i/cols)*_cols ] = matrix[i]; } else
        if ( dir==SET_BY_COLS ) { _matrix[(i/rows)+(i%rows)*_cols] = matrix[i]; }
    }

    return *this;
}

/**
* @brief Set value
* @param col
* @param row
* @param val
*/
CMatrix & CMatrix::setAt(int col, int row, double val)
{
    _matrix[col+row*_cols] = val;
    return *this;
}

/**
* @brief Get value
* @param col
* @param row
* @return
*/
double CMatrix::getAt(int col, int row) const
{
    return _matrix[col+row*_cols];
}

/**
* @brief Translate
* @param tx - offset by X axis
* @param ty - offset by Y axis
* @return
*/
CMatrix & CMatrix::translate(double tx, double ty)
{
    CMatrix mt(3, 3, {1, 0, tx, 0, 1, ty, 0, 0, 1});
    multiplication(mt);
    return *this;
}

/**
* @brief Scale
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
* @brief Rotate
* @param angle - angle in radians
* @return
*/
CMatrix& CMatrix::rotate(double angle)
{
    CMatrix mr(3, 3, {cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1});
    multiplication(mr);
    return *this;
}

/**
* @brief Rotate
* @param angle - angle in degrees
* @return
*/
CMatrix &CMatrix::rotateD(double angle)
{
    return rotate(angle*(M_PI/180.0));
}

/**
* @brief Addition
* @param m
* @return
*/
CMatrix & CMatrix::addition(const CMatrix & m)
{
    if ( size()!=m.size() ) { throw 20; }

    for (int i=0; i<_cols*_rows; ++i) {
        _matrix[i] += m._matrix[i];
    }

    return *this;
}

/**
* @brief Substraction
* @param m
* @return
*/
CMatrix & CMatrix::subtraction(const CMatrix & m)
{
    if ( size()!=m.size() ) { throw 20; }
    for (int i=0; i<_cols*_rows; ++i) {
        _matrix[i] -= m._matrix[i];
    }
    return *this;
}

/**
* @brief Set to identity matrix
* @return
*/
CMatrix &CMatrix::toIdentity()
{
    _matrix.clear();
    for (int i=0; i<_cols*_rows; ++i) {
        _matrix.append((i%_cols==i/_rows )? 1.0 : 0.0);
    }
    return *this;
}

CMatrix& CMatrix::toZeros()
{
    _matrix.clear();
    for (int i=0; i<_cols*_rows; ++i) {
        _matrix.append(0.0);
    }
    return *this;
}


/**
* @brief Multiplication matrix
* @param m
* @return
*/
CMatrix & CMatrix::multiplication(const CMatrix & m)
{
    if ( _rows!=m._cols ) { throw 21; }

    int cols = m._cols;
    int rows = _rows;

    TMatrix mn(cols*rows);

    for (int j=0; j<_rows; ++j) {
        for (int i=0; i<cols; ++i) {
            mn[i+j*cols] = 0;
            for (int l=0; l<_cols; ++l) {
                mn[i+j*cols] += _matrix[l+j*_cols] * m._matrix[i+l*cols];
            }
        }
    }

    _cols = cols;
    _rows = rows;
    _matrix = mn;

    return *this;
}

/**
* @brief Multiplication scalar
* @param n
* @return
*/
CMatrix &CMatrix::multiplication(double n)
{
    for (int i=0; i<_cols*_rows; ++i) {
        _matrix[i] *= n;
    }
    return *this;
}

/**
* @brief Deep clone matrix
* @return
*/
CMatrix CMatrix::clon() const
{
    CMatrix m2 = CMatrix::zeros(_cols, _rows);
    for (int i=0; i<_cols*_rows; ++i) {
        m2._matrix[i] = _matrix[i];
    }
    return m2;
}

/**
* @brief Apply matrix
* @param m
* @return
*/
CMatrix CMatrix::apply(const CMatrix & m) const
{
    if ( _rows!=m._rows ) { throw 22; }
    CMatrix r = CMatrix::zeros(1, m._rows);

    for (int i=0; i<_cols*_rows; ++i) {
        r._matrix[i/_cols] += _matrix[i]* m._matrix[i%_rows];
    }

    return r;
}

/**
* @brief Is matrix identity
* @return
*/
bool CMatrix::isIdentity() const
{
    for (int i=0; i<_rows*_cols; ++i) {
        if ( !Equal::almostEqual(_matrix[i], (i%_cols==i/_rows)? 1.0 : 0.0) ) { return false; }
    }
    return true;
}

/**
* @brief Is matrix zeros
* @return
*/
bool CMatrix::isZeros() const
{
    for (int i=0; i<_rows*_cols; ++i) {
        if ( !Equal::almostEqual(_matrix[i], 0.0) ) { return false; }
    }
    return true;
}

/**
* @brief operator []
* @param idx
* @return
*/
double& CMatrix::operator[](int idx)
{
    return _matrix[idx];
}

bool CMatrix::operator==(const CEmptyPriv&) const
{
    return (isIdentity() || isZeros());
}

/**
* @brief operator []
* @param idx
* @return
*/
double CMatrix::operator[](int idx) const
{
    return _matrix[idx];
}

/**
* @brief Cofactor
* @param p - col
* @param q - row
* @return
*/
CMatrix CMatrix::cofactor(int p, int q) const
{
    CMatrix cfm = CMatrix::zeros(_cols-1, _rows-1);
    for (int i=0, t=0; i<_cols*_rows; ++i) {
        if ( i%_cols==q || i/_rows==p ) { continue; }
        cfm[t++] = _matrix[i];
    }
    return cfm;
}

/**
* @brief CMatrix::determinant
* @return
*/
double CMatrix::determinant() const
{
    if ( _cols!=_rows ) { throw 23; }
    int mn = _cols;
    if ( mn==1 ) { return _matrix[0]; } //-- Base case : if matrix contains single element

    double d = 0;
    int sign = 1;
    for (int f = 0; f<mn; ++f) {
        CMatrix tmp = cofactor(f, 0);
        d += sign * _matrix[f*mn] * tmp.determinant();
        sign = -sign;
    }

    return d;
}

/**
* @brief Adjoint (must be a squere!)
* @param adj
*/
CMatrix CMatrix::adjoint() const
{
    CMatrix adj = CMatrix::zeros(_cols, _rows);
    if ( _cols!=_rows ) { throw 23; }
    int mn = _cols;
    if ( mn==1 ) { adj[0] = 1.0; return adj; }
    int sign = 1;
    for (int i=0; i<_cols*_rows; ++i) {
        CMatrix tmp = cofactor(i%mn, i/mn);
        sign = ((i%mn+i/mn)%2==0)? 1: -1;
        adj[i] = sign * tmp.determinant();
    }
    return adj;
}

/**
* @brief Inverse matrix (must be a square!)
* @return
*/
CMatrix& CMatrix::inverse()
{
    if ( _cols!=_rows ) { throw 23; }

    double det = determinant();
    if ( det==0 ) { throw 23; }

    CMatrix adj = adjoint();

    for (int i=0; i<_cols*_rows; ++i) {
        _matrix[i] = adj[i] / det;
    }

    return *this;
}

/**
* @brief qDebug()<<"matrix"<<m;
* @param dbg
* @param m
* @return
*/
QDebug operator<<(QDebug dbg, const CMatrix & m)
{
    dbg<<"CMatrix("<<Qt::endl;
    for (int i=0;  i< m._cols*m._rows; ++i) {
        dbg.noquote()<<QString::number(m._matrix[i]).leftJustified(10, ' ');
        if ( (i+1)%m._cols==0 ) { dbg<<Qt::endl; }
    }
    dbg<<")"<<Qt::endl;
    return dbg;
}
