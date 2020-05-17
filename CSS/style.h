#ifndef STYLE_H
#define STYLE_H

#include <QVariant>
#include <QString>
#include <QHash>
#include <QColor>
/**
* @brief Стили, которые есть в боке (Block)
*/
namespace CSS {

class Style
{
public:
    Style();

    void set(QString styleName, QString value);
    QVariant get(QString styleName);

    bool unite(const Style & other);

    operator QString() const;

    QString toString() const;

protected:
    typedef QHash<QString, QVariant> TStyles;
    TStyles _styles;

};
}
#endif // STYLE_H
