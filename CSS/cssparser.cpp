#include "cssparser.h"
#include <QRegExp>
#include <QDebug>

using namespace CSS;
CssParser::CssParser(QObject *parent) : QObject(parent)
{

}

bool CssParser::parse(QString styles)
{
    _blocks.clear();

    //-- Удалим всё лишнее
    styles = styles.remove(QRegExp("(\\n|\\t)"));

    qDebug()<<"Styles"<<styles;

    QRegExp rxStyles("([\\.#a-zA-Z0-9 ]+)\\{(.+)\\}");

    int posStyles = 0;
    while (( posStyles = rxStyles.indexIn(styles, posStyles)) != -1) {
        posStyles+= rxStyles.matchedLength();

        Block block(rxStyles.cap(2));
        if (block.parse()) {
            _blocks.insert(rxStyles.cap(1), block);
        }
    }

    qDebug()<<"blocks"<<_blocks;

    return true;
}

/**
* @brief Вычисляем все стили для идентификатора с наложением block
* @param identifier
* @param priorityBlock - будут наложены после вычисленных
* @return Style
*/
Style CssParser::applyStyles(QString identifier, const Block &priorityBlock)
{
    Block block = _blocks.value(identifier, Block());

    block.unite(priorityBlock);

    return static_cast<Style>(block);
}
