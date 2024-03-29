#ifndef MEASUREUNIT_H
#define MEASUREUNIT_H

#include <QMetaType>

namespace CSS {

/**
* @brief Значение с единицами измерения
*/
class MeasureUnit
{
public:

    enum TMeasureUnitType {
        MU_PT,
        MU_PX,
        MU_PERCENT
    };

    MeasureUnit();
    MeasureUnit(const MeasureUnit &other);
    MeasureUnit(double val, TMeasureUnitType type = MU_PT);
    ~MeasureUnit();

    void setVal(double val);
    void setType(TMeasureUnitType type);

    double val() const;
    TMeasureUnitType type() const;

    operator QString() const;
    MeasureUnit & operator =(const MeasureUnit &other);

    int asPx() const;

private:
    double _val;
    TMeasureUnitType _type;
};

}

Q_DECLARE_METATYPE(CSS::MeasureUnit)

#endif // MEASUREUNIT_H
