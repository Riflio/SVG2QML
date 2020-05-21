#include "cssparser.h"
#include <QRegExp>
#include <QDebug>

using namespace CSS;
CssParser::CssParser(QObject *parent) : QObject(parent)
{

}

/**
* @brief Парсим стили
* @param styles
* @return
*/
bool CssParser::parse(QString styles)
{
    //-- Удалим всё лишнее, а то регулярка выростает шибко
    styles = styles.remove(QRegExp("(\\n|\\t)"));

    QRegExp rxStyles("(?:\\}|^)?(.+)\\{(.+)\\}");
    rxStyles.setMinimal(true);

    int posStyles = 0;
    while ( (posStyles=rxStyles.indexIn(styles, posStyles))!=-1 ) {
        posStyles+= rxStyles.matchedLength();

        QStringList tokens = rxStyles.cap(1).split(',');
        QString tokenStyles = rxStyles.cap(2);

        foreach (QString token, tokens) {
            Block block(tokenStyles);

            if ( block.parse() ) {
                if ( _blocks.contains(token) ) {
                    _blocks[token].unite(block);
                } else {
                    _blocks.insert(token, block);
                }
            } else {
                qWarning()<<"Unable parse styles"<<token<<tokenStyles;
            }
        }
    }

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
