#ifndef QMLGENERATOR_H
#define QMLGENERATOR_H

#include "SVG/Generator/svggenerator.h"

class QMLGenerator : public SVGGenerator
{
    Q_OBJECT
public:
    explicit QMLGenerator(QObject *parent = nullptr);


    GenerateStatus generateQML(QIODevice *device, CPrimitive * rootItm);

signals:

private:
    QString tab(int c);

};

#endif // QMLGENERATOR_H
