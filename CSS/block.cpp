#include "block.h"

#include <QRegularExpression>
#include <QColor>

using namespace CSS;

Block::Block(QString block)
{
    _block = block;
}

bool Block::parse()
{
    QRegularExpressionMatchIterator rxStyles = QRegularExpression("([a-zA-Z-]+):([^;]+)").globalMatch(_block); //-- Парсим стили (ключ-значение разделяем по ";" )

    while ( rxStyles.hasNext() ) {
        QRegularExpressionMatch match = rxStyles.next();
        QString val = match.captured(2);
        set(match.captured(1), val);
    }

    return true;
}

