#include "block.h"

#include <QRegExp>
#include <QColor>
#include <QDebug>

using namespace CSS;

Block::Block(QString block)
{
    _block = block;
}

bool Block::parse()
{

    QRegExp rxStyles("([a-zA-Z-]+):([^;]+)"); //-- Парсим стили (ключ-значение разделяем по ";" )

    int posStyles = 0;
    while (( posStyles = rxStyles.indexIn(_block, posStyles)) != -1) {
        posStyles+= rxStyles.matchedLength();
        QString val = rxStyles.cap(2);        
        set(rxStyles.cap(1), val);
    }

    return true;
}

