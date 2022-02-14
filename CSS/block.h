#ifndef BLOCK_H
#define BLOCK_H

#include "style.h"
#include <QString>

/**
* @brief Парсим всё, что между {...}.
*/
namespace CSS {
class Block: public Style
{
public:
    Block();
    bool parse(QString block);

protected:

};
}
#endif // BLOCK_H
