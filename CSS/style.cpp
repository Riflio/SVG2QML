#include "style.h"
#include <QRegExp>
#include <QDebug>

using namespace CSS;

Style::Style()
{

}


void Style::set(QString styleName, QString value)
{
    //-- Известные для Qt типы сразу парсим в нативные значения
    QRegExp rxColorPercent("rgb\\(([0-9\\.]+)(%?),([0-9\\.]+)(%?),([0-9\\.]+)(%?)\\)"); //-- Регулярка для цвета в формате rgb(a,b,c) или rgb(a%,b%,c%)
    QRegExp rxColorHex("(#[0-9a-fA-F]{3,6})"); //-- Регулярка для цвета в формате hex

    QVariant vr;

    if (rxColorPercent.indexIn(value) > -1) { //-- Проверяем, не цвет ли это в rgb
        vr = QVariant(QColor(
            (rxColorPercent.cap(2)=="%")? rxColorPercent.cap(1).toDouble()*2.55 : rxColorPercent.cap(1).toInt(), //-- Если есть проценты, то переводим с лимитом в 255
            (rxColorPercent.cap(4)=="%")? rxColorPercent.cap(3).toDouble()*2.55 : rxColorPercent.cap(3).toInt(),
            (rxColorPercent.cap(6)=="%")? rxColorPercent.cap(5).toDouble()*2.55 : rxColorPercent.cap(5).toInt()
        ));
    } else
    if (rxColorHex.indexIn(value) > -1) { //-- Или hex
        vr = QVariant(QColor(rxColorHex.cap(1)));
    } else
    { //-- Ничего не подходит, ставим как есть
        vr = value;
    }

    _styles.insert(styleName, vr);
}

QVariant Style::get(QString styleName)
{
    return _styles.value(styleName, QVariant());
}

/**
* @brief Объединяем стили
* @param other
* @return
*/
bool Style::unite(const Style &other)
{
    _styles = _styles.unite(other._styles);
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



