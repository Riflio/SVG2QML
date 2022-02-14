#include "cssparser.h"
#include <QRegularExpression>

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
    styles = styles.remove(QRegularExpression("(\\n|\\t)"));

    QRegularExpression rxStyles("(?:\\}|^)?(.+)\\{(.+)\\}");
    rxStyles.setPatternOptions(QRegularExpression::InvertedGreedinessOption);

    QRegularExpressionMatchIterator i = rxStyles.globalMatch(styles);

    while ( i.hasNext() ) {
        QRegularExpressionMatch match = i.next();

        QStringList tokens = match.captured(1).split(',');
        QString tokenStyles = match.captured(2);

        foreach (QString token, tokens) {
            Block block;

            if ( block.parse(tokenStyles) ) {
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
