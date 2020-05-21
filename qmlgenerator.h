#ifndef QMLGENERATOR_H
#define QMLGENERATOR_H

#include "SVG/Generator/svggenerator.h"

class QMLGenerator : public SVGGenerator
{
    Q_OBJECT
public:
    explicit QMLGenerator(QObject *parent = nullptr);


    GenerateStatus generateQML(QIODevice *device, CPrimitive * rootItm, const CDefs &defs);

signals:

private:
    CPrimitive * _rootItm;
    CDefs _defs;

    QString tab(int c);
    QString sanitizeID(QString id);

    void makeFill(CPrimitive * itm, int &lvl, QTextStream &qml);
    void makeStroke(CPrimitive * itm, int &lvl, QTextStream &qml);
    void makeGradientStops(FGradient * gr, int &lvl, QTextStream &qml);

    void makeElement(CPrimitive * el, int &lvl, QTextStream &qml, const QString &rootID, bool firstInline=false);

};

#endif // QMLGENERATOR_H
