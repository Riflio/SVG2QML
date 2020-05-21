#include "measureunit.h"
#include <QDebug>

using namespace CSS;

MeasureUnit::MeasureUnit()
{

}

MeasureUnit::~MeasureUnit()
{

}

MeasureUnit::MeasureUnit(const MeasureUnit &other)
{
    _val = other._val;
    _type = other._type;
}

CSS::MeasureUnit::MeasureUnit(double val, CSS::MeasureUnit::TMeasureUnitType type): _val(val), _type(type)
{

}

void MeasureUnit::setVal(double val)
{
    _val = val;
}

void MeasureUnit::setType(MeasureUnit::TMeasureUnitType type)
{
    _type = type;
}

double CSS::MeasureUnit::val() const
{
    return _val;
}

MeasureUnit::TMeasureUnitType MeasureUnit::type() const
{
    return _type;
}

/**
* @brief Преобразуем в пиксели
* @return
*/
int MeasureUnit::asPx() const
{
    if ( _type==MU_PX ) return _val;
    if ( _type==MU_PT ) return _val*1.827; //TODO: Разобраться, как правильно переводить...

    qWarning()<<"Unsupported conversion MeasureUnit from"<<_type<<"to px.";
    return 0;
}

/**
* @brief Выводим строкой (для дебага)
*/
CSS::MeasureUnit::operator QString() const
{
    return QString("MeasureUnit(%1, %2)").arg(val()).arg(type());
}
