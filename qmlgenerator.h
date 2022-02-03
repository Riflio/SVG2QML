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
        bool enableScale = false; //-- Использовать или нет свойство scale
    };

    void setSettings(TSettings settings);
    TSettings settings() const;

    GenerateStatus generateQML(QIODevice *device, CPrimitive * rootItm, const CDefs &defs);

signals:

private:
    QTextStream _qml;
    int _lvl;

    TSettings _settings;
    CPrimitive * _rootItm;
    CDefs _defs;

    QString tab(int c);
    QString sanitizeID(QString id);

    QString primitiveToPathCommands(CPrimitive * p, double offset=0);

    bool makeFill(CPrimitive * itm, bool isSimple);

    bool makeOpacity(CPrimitive * itm);

    void makeFillGradient(CPrimitive * itm, FGradient * gr, CDef::TDefType type);
    void makeFillGradientTransform(CPrimitive * itm, FGradient * gr, CDef::TDefType type);
    void makeRadialGradient(FRadialGradient * gr);
    void makeLinearGradient(FLinearGradient * gr);
    void makeGradientStops(FGradient * gr);

    void makeStroke(CPrimitive * itm);

    void makeElement(CPrimitive * el, bool visible=true, bool layerEnabled=false);

    void makeTransform(const CMatrix &m);

    void makeID(CPrimitive * itm);

    void writeStartLvlProp(QString propName, QString elementName);
    void writeStartLvl(QString elementName, bool isInline=false);
    void writeEndLvl(bool isInline=false);
    void writePropVal(QString name, QVariant val, bool quoted=false, bool isInline=false);
    void writePropThinkLinesVal(QString name, QVariant valThink, QVariant val, bool quoted=false, bool isInline=false);
    void writePropElVal(QString name, QString elementName, QVariant val);
};

#endif // QMLGENERATOR_H
