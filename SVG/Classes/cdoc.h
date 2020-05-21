#ifndef CDOC_H
#define CDOC_H

#include "../Assets/cprimitive.h"

/**
* @brief Корневой элемент после парсинга
*/
class CDoc: public CPrimitive
{
public:
    CDoc();

    void setTitle(QString title);
    QString title() const;

private:
    QString _title;
};

#endif // CDOC_H
