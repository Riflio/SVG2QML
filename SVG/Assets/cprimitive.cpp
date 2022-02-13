#include "cprimitive.h"

template<class T>
CPrimitive<T>::CPrimitive(const CPrimitive&other)
    : CNodeInterface(other), _type(other._type), _points(other._points), _styles(other._styles), _boundingBox(other._boundingBox), _id(other._id),
      _className(other._className), _transformMatrix(other._transformMatrix), _title(other._title), _descr(other._descr)
{

}

template<class T>
CPrimitive<T>::CPrimitive(): CNodeInterface(), _type(PT_NONE), _id("")
{
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(PrimitiveType type): CPrimitive()
{
    _type = type;
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(PrimitiveType type, const CPoint &p1): CPrimitive(type)
{
    _points.add(p1);
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2): CPrimitive(type)
{
    _points.add(p1);
    _points.add(p2);
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4): CPrimitive(type)
{
    _points.add(p1);
    _points.add(p2);
    _points.add(p3);
    _points.add(p4);
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5, const CPoint &p6): CPrimitive(type)
{
    _points.add(p1);
    _points.add(p2);
    _points.add(p3);
    _points.add(p4);
    _points.add(p5);
    _points.add(p6);
    needUpdate();
}

template<class T>
CPrimitive<T>::CPrimitive(CPrimitive::PrimitiveType type, const CPoints &points): CPrimitive(type)
{
    _points = points;
    needUpdate();
}

template<class T>
CPrimitive<T>::~CPrimitive()
{
    for (INodeInterface * ni=this->down(); ni!=nullptr; ni=ni->next()) {
        delete ni;
    }
    removeFromLevel();    
}

template<class T>
typename CPrimitive<T>::PrimitiveType CPrimitive<T>::type() const
{
    return _type;
}

template<class T>
CPoints CPrimitive<T>::points() const
{
    return _points;
}

template<class T>
void CPrimitive<T>::setPoints(const CPoints &points)
{
    _points = points;
    needUpdate();
}

template<class T>
int CPrimitive<T>::pointsCount() const
{
    return _points.count();
}

template<class T>
CSS::Style CPrimitive<T>::styles() const
{
    return _styles;
}

/**
* @brief Return bounding gox (with childs)
* @param withTransform - consider transformation matrix
* @return CBoundingBox
*/
template<class T>
CBoundingBox CPrimitive<T>::boundingBox(bool withTransform) const
{
    if ( !_boundingBox.isEmpty() ) { return _boundingBox; }
    _boundingBox.addPoints(_points);

    if ( withTransform ) { _boundingBox.transform(_transformMatrix); }

    //-- Суммируем все ограничительные рамки у вложенных итемов    
    for (INodeInterface *ni = down(); ni!=nullptr; ni=ni->next()) {
        IPrimitive * pi = dynamic_cast<IPrimitive*>(ni);
        _boundingBox.addBBox(pi->boundingBox());
    }


    return _boundingBox;
}

template<class T>
void CPrimitive<T>::setStyles(CSS::Style styles)
{
    _styles = styles;
}

/**
* @brief Изменяем направление на противоположное
*/
template<class T>
void CPrimitive<T>::reverse()
{
    _points.reverse();
}

/**
* @brief Превращаем в набор отрезков
* @param tol
* @return
*/
template<class T>
CPoints CPrimitive<T>::lianirize(double tol) const
{
    CPoints res;
    for (INodeInterface * ni = down(); ni!=nullptr; ni=ni->next()) {
        IPrimitive * pr = dynamic_cast<IPrimitive*>(ni);
        res.append(pr->lianirize(tol));
    }
    return res;
}



/**
* @brief Clone nesteed
*/
template<class T>
CPrimitive<T> * CPrimitive<T>::clone() const
{    
    T * clon = new T(*dynamic_cast<const T*>(this));

    clon->reset();

    for (INodeInterface * ni=this->down(); ni!=nullptr; ni=ni->next()) {
        IPrimitive * pr = dynamic_cast<IPrimitive*>(ni);
        IPrimitive * prCopy = pr->clone();
        clon->addNext(prCopy);
    }

    return clon;
}

/**
* @brief Применяем заданную матрицу трансформаций
* @param matrix
* @return
*/
template<class T>
bool CPrimitive<T>::applyTransform(const CMatrix &matrix)
{
    //-- Себя
    for (int i=0; i<_points.count(); ++i) {
        _points[i].transform(matrix);
    }

    needUpdate();

    //-- Остальным на нижнем уровне, если есть
    if ( this->down()!=nullptr ) {
        for (INodeInterface * ni=this->down(); ni!=nullptr; ni=ni->next()) {
            IPrimitive * pi = dynamic_cast<IPrimitive*>(ni);
            pi->applyTransform(matrix);
        }
    }

    return true;
}

/**
* @brief Преобразовываем себя в путь и добавляем к себе же уровнем ниже
* @return
*/
template<class T>
bool CPrimitive<T>::toPath()
{
    return false;
}

template<class T>
QString CPrimitive<T>::ID() const
{
    return _id;
}

template<class T>
void CPrimitive<T>::setID(QString id)
{
    _id = id;
}

template<class T>
QString CPrimitive<T>::className() const
{
    return _className;
}

template<class T>
void CPrimitive<T>::setClassName(QString className)
{
    _className = className;
}

template<class T>
void CPrimitive<T>::setTitle(QString title)
{
    _title = title;
}

template<class T>
QString CPrimitive<T>::title() const
{
    return _title;
}

template<class T>
void CPrimitive<T>::setDescr(QString descr)
{
    _descr = descr;
}

template<class T>
QString CPrimitive<T>::descr() const
{
    return _descr;
}

template<class T>
CPoint &CPrimitive<T>::operator[](int i)
{
    return _points[i];
}

template<class T>
const CPoint &CPrimitive<T>::operator[](int i) const
{
    return _points[i];
}

/**
* @brief Сбрасываем все запомненные расчёты
*/
template<class T>
void CPrimitive<T>::needUpdate()
{
    _boundingBox.clear();

    //-- Придётся и у предка то же, т.к. может быть основан на наших
    IPrimitive * upItm = dynamic_cast<IPrimitive*>(up());
    if ( upItm!=nullptr ) { upItm->needUpdate(); }
}

template<class T>
CMatrix CPrimitive<T>::transformation() const
{
    return _transformMatrix;
}

template<class T>
void CPrimitive<T>::setTransformation(const CMatrix &matrix)
{
    _transformMatrix = matrix;
}
