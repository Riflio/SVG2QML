#ifndef STYLE_H
#define STYLE_H

#include <QVariant>
#include <QString>
#include <QHash>
#include <QColor>
#include <QUrl>

#include "measureunit.h"

/**
* @brief Стили, которые есть в боке (Block)
*/
namespace CSS {

class Style
{
public:
    Style();

    void set(QString styleName, QString value);
    QVariant get(QString styleName) const;
    bool has(const QString &key) const;

    bool unite(const Style & other);

    operator QString() const;

    QString toString() const;

    static QColor parseColor(QString color);

protected:
    typedef QHash<QString, QVariant> TStyles;
    TStyles _styles;

};
}
#endif // STYLE_H
