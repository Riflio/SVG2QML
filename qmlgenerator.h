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

    QString primitiveToPathCommands(CPrimitive * p);

    bool makeFill(CPrimitive * itm, int &lvl, QTextStream &qml, const QString &rootID, bool isSimple);

    void makeFillGradient(CPrimitive * itm, FGradient * gr, int &lvl, QTextStream &qml, const QString &rootID, CDef::TDefType type);
    void makeFillGradientTransform(CPrimitive * itm, FGradient * gr, int &lvl, QTextStream &qml, const QString &rootID, CDef::TDefType type);
    void makeRadialGradient(FRadialGradient * gr, int &lvl, QTextStream &qml, const QString &rootID);
    void makeLinearGradient(FLinearGradient * gr, int &lvl, QTextStream &qml, const QString &rootID);
    void makeGradientStops(FGradient * gr, int &lvl, QTextStream &qml, const QString &rootID);

    void makeStroke(CPrimitive * itm, int &lvl, QTextStream &qml, const QString &rootID);

    void makeElement(CPrimitive * el, int &lvl, QTextStream &qml, const QString &rootID, bool firstInline=false);

    void makeID(CPrimitive * itm, int &lvl, QTextStream &qml);
};

#endif // QMLGENERATOR_H
