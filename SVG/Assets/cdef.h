#ifndef CDEF_H
#define CDEF_H

#include <QObject>

/**
* @brief Базовый клас для шаблонных элементов
*/
class CDef : public QObject
{
    Q_OBJECT
public:
    explicit CDef(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CDEF_H
