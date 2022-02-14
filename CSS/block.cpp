#include "block.h"

#include <QRegularExpression>
#include <QColor>

using namespace CSS;

Block::Block() {}


bool Block::parse(QString block)
{
    QRegularExpressionMatchIterator rxStyles = QRegularExpression("([a-zA-Z-]+):([^;]+)").globalMatch(block); //-- Парсим стили (ключ-значение разделяем по ";" )

    while ( rxStyles.hasNext() ) {
        QRegularExpressionMatch match = rxStyles.next();
        QString val = match.captured(2);
        set(match.captured(1), val);
    }

    return true;
}

