#ifndef QMLGENERATOR_H
#define QMLGENERATOR_H

#include "SVG/Generator/svggenerator.h"

class QMLGenerator : public SVGGenerator
{
    Q_OBJECT
public:
    explicit QMLGenerator(QObject *parent = nullptr);

    struct TSettings {
        QString rootName = "svg2qml";
        QString versionQtQuick = "2.12";
        QString versionQtQuickShapes = "1.12";
        QString versionQtGraphicalEffects = "1.12";
        bool enableScale = false; //-- Использовать или нет свойство scale
    };

    void setSettings(TSettings settings);
    TSettings settings() const;

    GenerateStatus generateQML(QIODevice *device, CPrimitive * rootItm, const CDefs &defs);

signals:

private:
    TSettings _settings;
    CPrimitive * _rootItm;
    CDefs _defs;

    QString tab(int c);
    QString sanitizeID(QString id);

    QString primitiveToPathCommands(CPrimitive * p, double offset=5);

    bool makeFill(CPrimitive * itm, int &lvl, QTextStream &qml, bool isSimple);

    bool makeOpacity(CPrimitive * itm, int &lvl, QTextStream &qml);

    void makeFillGradient(CPrimitive * itm, FGradient * gr, int &lvl, QTextStream &qml, CDef::TDefType type);
    void makeFillGradientTransform(CPrimitive * itm, FGradient * gr, int &lvl, QTextStream &qml, CDef::TDefType type);
    void makeRadialGradient(FRadialGradient * gr, int &lvl, QTextStream &qml);
    void makeLinearGradient(FLinearGradient * gr, int &lvl, QTextStream &qml);
    void makeGradientStops(FGradient * gr, int &lvl, QTextStream &qml);

    void makeStroke(CPrimitive * itm, int &lvl, QTextStream &qml);

    void makeElement(CPrimitive * el, int &lvl, QTextStream &qml, bool firstInline=false);

    void makeTransform(const CMatrix &m, int &lvl, QTextStream &qml);

    void makeID(CPrimitive * itm, int &lvl, QTextStream &qml);
};

#endif // QMLGENERATOR_H
