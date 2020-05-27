#include "cprimitive.h"

CPrimitive::CPrimitive(const CPrimitive&other)
    :CNodeInterface(other), rotation(other.rotation), offset(other.offset), marked(other.marked), source(other.source),
      flippedX(false), flippedY(false), _type(other._type), _points(other._points), _styles(other._styles), _bbox(other._bbox), _id(other._id)
{    
}

CPrimitive::CPrimitive():CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false), _type(PT_NONE), _id("")
{
    needUpdate();
}

CPrimitive::CPrimitive(PrimitiveType type)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false), _type(type), _id("")
{
    needUpdate();
}


CPrimitive::CPrimitive(PrimitiveType type, const CPoint &p1)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false),_type(type), _id("")
{
    _points.add(p1);
    needUpdate();
}


CPrimitive::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false),_type(type), _id("")
{
    _points.add(p1);
    _points.add(p2);
    needUpdate();
}

CPrimitive::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false), _type(type), _id("")
{
    _points.add(p1);
    _points.add(p2);
    _points.add(p3);
    _points.add(p4);
    needUpdate();
}

CPrimitive::CPrimitive(PrimitiveType type, const CPoint &p1, const CPoint &p2, const CPoint &p3, const CPoint &p4, const CPoint &p5, const CPoint &p6)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false), _type(type), _id("")
{
    _points.add(p1);
    _points.add(p2);
    _points.add(p3);
    _points.add(p4);
    _points.add(p5);
    _points.add(p6);
    needUpdate();
}

CPrimitive::CPrimitive(CPrimitive::PrimitiveType type, const CPoints &points)
    : CNodeInterface(), rotation(0), offset(CPoint(0,0)), marked(false), source(0), flippedX(false), flippedY(false), _type(type),  _points(points), _id("")
{

}

CPrimitive::~CPrimitive()
{
    CNodeInterface::removeFromLevel(this);
}

CPrimitive::PrimitiveType CPrimitive::type() const
{
    return _type;
}

CPoints CPrimitive::points() const
{
    return _points;
}

void CPrimitive::setPoints(const CPoints &points)
{
    _points = points;
    needUpdate();
}

int CPrimitive::pointsCount() const
{
    return _points.count();
}

/**
* @brief Прорисовываем на своём уровне
* @param painter
* @param scale
*/
void CPrimitive::cpaint(QPainter *painter, const CBoundingBox &area)
{
    for (CNodeInterface * ni = down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pr = static_cast<CPrimitive*>(ni);
        pr->cpaint(painter, area);
    }
}

CSS::Style CPrimitive::styles() const
{
    return _styles;
}

/**
* @brief Отдаём ограничительную рамку элемента (с учётом вложенных)
* @return CBoundingBox*
*/
const CBoundingBox &CPrimitive::getBBox() const
{    
    if ( !_bbox.isEmpty() ) return _bbox;

    _bbox.addPoints(_points);

    //-- Суммируем все ограничительные рамки у вложенных итемов    
    for (CNodeInterface *ni = down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pi = static_cast<CPrimitive*>(ni);
        _bbox.addBBox(pi->getBBox());
    }

    return _bbox;
}

void CPrimitive::setStyles(CSS::Style styles)
{
    _styles = styles;
}

/**
* @brief Изменяем направление на противоположное
*/
void CPrimitive::reverse()
{
    _points.reverse();
}

/**
* @brief Превращаем в набор отрезков
* @param tol
* @return
*/
CPoints CPrimitive::lianirize(double tol) const
{
    CPoints res;
    for (CNodeInterface * ni = down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pr = static_cast<CPrimitive*>(ni);
        res.append(pr->lianirize(tol));
    }
    return res;
}

/**
* @brief Перемещаем примитив на задданое расстояние по XY
* @param m
*/
void CPrimitive::move(const CPoint &m)
{
    _points.move(m);
    needUpdate();

    //-- И потомков ниже уровнем
    for (CNodeInterface * ni=this->down; ni!=nullptr; ni = ni->next) {
        CPrimitive * p = static_cast<CPrimitive*>(ni);
        p->move(m);
    }
}

/**
* @brief Удаляем примитив и всех на своём уровне
*/
void CPrimitive::del()
{
    for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
        delete ni;
    }
    delete this;
}

/**
* @brief Создаём копию себя и всех на своём уровне
*/
CPrimitive * CPrimitive::copy(bool nesteed) const
{
    CPrimitive * meCopy = new CPrimitive(*this);
    CNodeInterface::reset(meCopy);

    if ( nesteed ) {
        for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
            CPrimitive * pr = static_cast<CPrimitive*>(ni);
            CPrimitive * prCopy = pr->copy();
            CNodeInterface::addNext(meCopy, prCopy);
        }
    }
    return meCopy;
}

/**
* @brief Вертим =)
* @param center - точка, вокруг которой вертим
* @param angle - угол в радианах
*/
void CPrimitive::rotate(const CPoint &center, double angle)
{
    rotation = angle;

    _points.rotate(center, angle);

    for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pi = static_cast<CPrimitive*>(ni);
        pi->rotate(center, angle);
    }

    needUpdate();
}

/**
* @brief Маштабируем
* @param sX - По X
* @param sY - По Y
*/
void CPrimitive::scale(double sX, double sY)
{
    if ( sX<0 ) flippedX = !flippedX;
    if ( sY<0 ) flippedY = !flippedY;

    //-- Себя
    for (int i=0; i<_points.count(); ++i) {
        _points[i].set(_points[i].x()*sX, _points[i].y()*sY);
    }

    //-- Остальных на нижнем уровне, если есть
    if ( this->down!=nullptr ) {
        for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
            CPrimitive * pi = static_cast<CPrimitive*>(ni);
            pi->scale(sX, sY);
        }
    }

    needUpdate(); //TODO: Можно просто так же маштабировать ограничительную рамку, если в дальнейшем только она из изменяемых останется
}

/**
* @brief Применяем заданную матрицу трансформаций
* @param matrix - если нужно применить ещё и какую-то другую
* @return
*/
bool CPrimitive::applyTransform(const CMatrix &matrix)
{
    CMatrix totalMatrix = _transformMatrix.multiplication(matrix);

    //-- Себя
    for (int i=0; i<_points.count(); ++i) {
        _points[i].transform(totalMatrix);
    }

    //-- Остальным на нижнем уровне, если есть
    if ( this->down!=nullptr ) {
        for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
            CPrimitive * pi = static_cast<CPrimitive*>(ni);
            pi->applyTransform(totalMatrix);
        }
    }

    _transformMatrix.clear();
    return true;
}

/**
* @brief Преобразовываем себя в путь и добавляем к себе же уровнем ниже
* @return
*/
bool CPrimitive::toPath()
{
    return false;
}

QString CPrimitive::ID() const
{
    return _id;
}

void CPrimitive::setID(QString id)
{
    _id = id;
}

QString CPrimitive::classSVG() const
{
    return _class;
}

void CPrimitive::setClassSVG(QString classSVG)
{
    _class = classSVG;
}

void CPrimitive::setTitle(QString title)
{
    _title = title;
}

QString CPrimitive::title() const
{
    return _title;
}

void CPrimitive::setDescr(QString descr)
{
    _descr = descr;
}

QString CPrimitive::descr() const
{
    return _descr;
}

CPoint &CPrimitive::operator[](int i)
{
    return _points[i];
}

const CPoint &CPrimitive::operator[](int i) const
{
    return _points[i];
}

/**
* @brief Ставим отметку выбран примитив или нет
* @param isSelected
*/
void CPrimitive::setSelected(bool isSelected)
{
    marked = isSelected;
    for (CNodeInterface * ni=this->down; ni!=nullptr; ni=ni->next) {
        CPrimitive * pi = static_cast<CPrimitive*>(ni);
        pi->setSelected(isSelected);
    }
}

/**
* @brief Сбрасываем все запомненные расчёты
*/
void CPrimitive::needUpdate()
{
    _bbox.clear();

    //-- Придётся и у предка то же, т.к. может быть основан на наших
    CPrimitive * upItm = static_cast<CPrimitive*>(up);
    if ( upItm!=nullptr ) upItm->needUpdate();
}

CMatrix CPrimitive::trandform() const
{
    return _transformMatrix;
}

void CPrimitive::setTransform(const CMatrix &matrix)
{
    _transformMatrix = matrix;
}

/**
* @brief Рисуем путь с настройками по умолчанию
* @param path
* @param painter
*/
void CPrimitive::drawPath(const QPainterPath &path, QPainter *painter)
{
    painter->save();
    QPen pen;

    if ( marked ) {
        pen.setColor(Qt::red);
        pen.setWidth(5);
    } else {
        pen.setColor( (styles().get("stroke").isValid())? styles().get("stroke").toString() : "#00ff00" );
        pen.setWidth(1);
    }

    painter->setPen(pen);
    painter->drawPath(path);

    painter->restore();
}
