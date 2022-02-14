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
    };

    void setSettings(TSettings settings);
    TSettings settings() const;

    GenerateStatus generateQML(QIODevice *device, IPrimitive * rootItm, const CDefs &defs);

    class QShape
    {
    public:
        QShape(IPrimitive * itm): itm(itm), x(0.0), y(0.0), r(0.0) {}
        IPrimitive * itm;
        QString pathCommands;
        CBoundingBox boundingBox;
        double ml;
        double mt;
        double x;
        double y;
        double r;
    };

signals:

private:
    QTextStream _qml;
    int _lvl;

    TSettings _settings;
    IPrimitive * _rootItm;
    CDefs _defs;

    QString tab(int c);
    QString sanitizeID(QString id);

    QString primitiveToPathCommands(IPrimitive * p, double offset=0, CMatrix transforms = CMatrix::identity(3, 3));

    bool makeFill(QShape * shape, bool isSimple);

    bool makeOpacity(QShape* shape);

    void makeFillGradient(QShape* shape, FGradient * gr, CDef::TDefType type);
    void makeFillGradientTransform(QShape* shape, FGradient * gr, CDef::TDefType type);
    void makeRadialGradient(FRadialGradient * gr);
    void makeLinearGradient(FLinearGradient * gr);
    void makeGradientStops(FGradient * gr);

    void makeStroke(QShape* shape);

    void makeElement(IPrimitive * el);

    void makeTransform(const CMatrix &m);

    void makeID(IPrimitive* itm);

    void writeStartLvlProp(QString propName, QString elementName);
    void writeStartLvl(QString elementName, bool isInline=false);
    void writeEndLvl(bool isInline=false);
    void writePropVal(QString name, QVariant val, bool quoted=false, bool isInline=false);
    void writePropThinkLinesVal(QString name, QVariant valThink, QVariant val, bool quoted=false, bool isInline=false);
    void writePropElVal(QString name, QString elementName, QVariant val);
};

#endif // QMLGENERATOR_H
