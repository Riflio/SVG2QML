#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <QObject>

#include <QFile>
#include <QXmlStreamReader>

#include "CSS/cssparser.h"

#include "Algebra/cmatrix.h"
#include "Algebra/cpoint.h"

#include "SVG/Classes/cdoc.h"
#include "SVG/Classes/csvg.h"
#include "SVG/Classes/cbezier.h"
#include "SVG/Classes/cline.h"
#include "SVG/Classes/cpath.h"
#include "SVG/Classes/cgroup.h"
#include "SVG/Classes/cimage.h"
#include "SVG/Classes/carc.h"
#include "SVG/Classes/ccircle.h"
#include "SVG/Classes/crect.h"
#include "SVG/Classes/cellipse.h"
#include "SVG/Classes/cpolyline.h"
#include "SVG/Classes/cpolygon.h"

#include "SVG/Classes/fclippath.h"
#include "SVG/Classes/flineargradient.h"
#include "SVG/Classes/fradialgradient.h"
#include "SVG/Classes/fprimitive.h"

#include "SVG/Assets/cdefs.h"


class SVGParser : public QObject
{
    Q_OBJECT

public:

    enum ParseStatus {
        PS_OK,
        PS_NOFILE,
        PS_SYNTAXERROR,
        PS_WRONGVERSION
    };

    explicit SVGParser(QObject *parent = nullptr);
    ParseStatus parse(QIODevice *device);

    CPrimitive * rootItem() const;

    CDefs defs() const;

    #define RX_PARAMS "([-+]?(\\d{1,20}|\\.)(\\.)?(\\d{1,20})?(e[+-]\\d{1,20})?)"

signals:

public slots:

private:
    CDoc * _rootItem;
    QXmlStreamReader * _xml;

    CPoint _globalCoords; //-- Глобальные координаты

    CSS::CssParser * _cssParser;

    CPrimitive * parseSVG(CNodeInterface **level, QXmlStreamReader * xml);
    CPrimitive * parseGroup(CNodeInterface **level, QXmlStreamReader * xml);
    CPrimitive * parsePath(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseRect(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseLine(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseImage(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseCircle(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseEllipse(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parsePolyline(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parsePolygon(CNodeInterface * level, QXmlStreamReader * xml);
    CPrimitive * parseClipPath(CNodeInterface ** level, QXmlStreamReader * xml);
    CPrimitive * parseLinearGradient(CNodeInterface ** level, QXmlStreamReader * xml);
    CPrimitive * parseRadialGradient(CNodeInterface ** level, QXmlStreamReader * xml);

    bool parseGradientStops(FGradient * gradient, QXmlStreamReader * xml);
    bool parseCss(CNodeInterface * level, QXmlStreamReader * xml);
    QList<double> parseParams(QString attr, QXmlStreamReader * xml) const;
    QList<double> parseParams(QString params) const;

    CMatrix parseTransform(QXmlStreamReader * xml, QString attrName="transform");
    CSS::Style parseStyle(QXmlStreamReader * xml);

    CDefs _defs;

    CDef * hasLink(QXmlStreamReader * xml);

    void parseBaseAttributes(CPrimitive * itm, QXmlStreamReader * xml);

};

#endif // SVGPARSER_H
