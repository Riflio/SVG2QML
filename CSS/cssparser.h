#ifndef CSSPARSER_H
#define CSSPARSER_H

#include <QObject>
#include <QString>

#include "CSS/block.h"

/**
* Парсим CSS
*/
namespace CSS {
class CssParser : public QObject
{
    Q_OBJECT
public:
    explicit CssParser(QObject *parent = nullptr);

    bool parse(QString styles);

    Style applyStyles(QString identifier, const Block &priorityBlock = Block());

signals:

public slots:

private:
    QMap<QString,Block> _blocks; //-- Все блоки. Ключ - идентификатор, значение - стили блока

};
}

#endif // CSSPARSER_H
