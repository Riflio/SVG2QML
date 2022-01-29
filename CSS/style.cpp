#include "style.h"
#include <QRegularExpression>

using namespace CSS;

Style::Style()
{

}

void Style::set(QString styleName, QString value)
{
    //-- Известные для Qt типы сразу парсим в нативные значения
    QRegularExpressionMatch rxURL = QRegularExpression("url\\((.+)\\)").match(value); //-- Для url()
    QRegularExpressionMatchIterator rxMeasureUnit = QRegularExpression("([0-9]+(\\.[0-9]+)?)(px|%)?").globalMatch(value); //-- Число с единицами измерения
    QVariant vr;

    QColor color = parseColor(value);

    if ( color.isValid() ) {
        vr = QVariant::fromValue(color);
    } else
    if ( rxURL.hasMatch() ) { //-- URL
        vr = QVariant::fromValue(QUrl(rxURL.captured(1)));
    } else
    if ( rxMeasureUnit.hasNext() ) { //-- Число с единицами измерения

        QList<MeasureUnit> muList;        
        while ( rxMeasureUnit.hasNext() ) {
            QRegularExpressionMatch match = rxMeasureUnit.next();

            MeasureUnit mu;
            mu.setVal(match.captured(1).toDouble());
            if ( match.captured(3)=="px" ) { mu.setType(MeasureUnit::MU_PX); }
            else if ( match.captured(3)=="%" ) { mu.setType(MeasureUnit::MU_PERCENT); }
            else { mu.setType(MeasureUnit::MU_PT); }
            muList.append(mu);
        }
        if ( muList.count()==1 ) {
            vr = QVariant::fromValue(muList[0]);
        } else {
            vr = QVariant::fromValue(muList);
        }

    } else { //-- Ничего не подходит, ставим как есть
        vr = value;
    }

    _styles.insert(styleName, vr);
}

QVariant Style::get(QString styleName) const
{
    return _styles.value(styleName, QVariant());
}

bool Style::has(const QString &key) const
{
    return _styles.contains(key);
}

/**
* @brief Объединяем стили
* @param other
* @return
*/
bool Style::unite(const Style &other)
{
    _styles.insert(other._styles);
    return true;
}

/**
* @brief Выводим одной строкой
* @return
*/
QString Style::toString() const
{
    QStringList res;
    for(TStyles::const_iterator i=_styles.constBegin(); i!=_styles.constEnd(); ++i) {
        res.append(QString("%1:%2;").arg(i.key()).arg(i.value().toString()));
    }
    return  res.join(" ");
}

/**
* @brief Парсим цвет
* @param color
* @return
*/
QColor Style::parseColor(QString value)
{
    QRegularExpressionMatch rxColorPercent = QRegularExpression("rgb\\(([0-9\\.]+)(%?),([0-9\\.]+)(%?),([0-9\\.]+)(%?)\\)").match(value); //-- Регулярка для цвета в формате rgb(a,b,c) или rgb(a%,b%,c%)
    QRegularExpressionMatch rxColorHex = QRegularExpression("(#[0-9a-fA-F]{3,6})").match(value); //-- Регулярка для цвета в формате hex

    QColor color;

    if ( rxColorPercent.hasMatch() ) { //-- Проверяем, не цвет ли это в rgb
        color = QColor(
            (rxColorPercent.captured(2)=="%")? rxColorPercent.captured(1).toDouble()*2.55 : rxColorPercent.captured(1).toInt(), //-- Если есть проценты, то переводим с лимитом в 255
            (rxColorPercent.captured(4)=="%")? rxColorPercent.captured(3).toDouble()*2.55 : rxColorPercent.captured(3).toInt(),
            (rxColorPercent.captured(6)=="%")? rxColorPercent.captured(5).toDouble()*2.55 : rxColorPercent.captured(5).toInt()
        );
    } else
    if ( rxColorHex.hasMatch() ) { //-- Или hex
        color = QColor(rxColorHex.captured(1));
    }

    return color;
}

/**
* @brief Для дебага
*/
CSS::Style::operator QString() const
{
    QString str;
    TStyles::const_iterator i = _styles.begin();
    while (i != _styles.end()) {
        str.append( QString("%1: %2; ").arg(i.key()).arg(i.value().toString()) );
        ++i;
    }
    return str;
}



