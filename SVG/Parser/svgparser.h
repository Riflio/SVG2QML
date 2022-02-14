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

    IPrimitive * rootItem() const;

    CDefs defs() const;

    #define RX_PARAMS "([-+]?(\\d{1,20}|\\.)(\\.)?(\\d{1,20})?(e[+-]\\d{1,20})?)"

signals:

public slots:

private:
    CDoc * _rootItem;
    QXmlStreamReader * _xml;

    CPoint _globalCoords; //-- Глобальные координаты

    CSS::CssParser * _cssParser;

    IPrimitive * parseSVG(INodeInterface **level, QXmlStreamReader * xml);
    IPrimitive * parseGroup(INodeInterface** level, QXmlStreamReader * xml);
    IPrimitive * parsePath(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parseRect(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parseLine(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parseImage(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parseCircle(INodeInterface* level, QXmlStreamReader * xml);
    IPrimitive * parseEllipse(INodeInterface* level, QXmlStreamReader * xml);
    IPrimitive * parsePolyline(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parsePolygon(INodeInterface * level, QXmlStreamReader * xml);
    IPrimitive * parseClipPath(INodeInterface ** level, QXmlStreamReader * xml);
    IPrimitive * parseLinearGradient(INodeInterface ** level, QXmlStreamReader * xml);
    IPrimitive * parseRadialGradient(INodeInterface ** level, QXmlStreamReader * xml);

    bool parseGradientStops(FGradient * gradient, QXmlStreamReader * xml);
    bool parseCss(INodeInterface* level, QXmlStreamReader * xml);
    QList<double> parseParams(QString attr, QXmlStreamReader * xml) const;
    QList<double> parseParams(QString params) const;

    CMatrix parseTransform(QXmlStreamReader * xml, QString attrName="transform");
    CSS::Style parseStyle(IPrimitive* itm, QXmlStreamReader * xml);

    CDefs _defs;

    void parseBaseAttributes(IPrimitive * itm, QXmlStreamReader * xml);
    void parseHREF(CDef * el, QXmlStreamReader * xml);

    QMultiHash<QString, CDef*> _dependsCDef; //-- Waiting href element depend will be parsed

    void updateDependsHREFS(CDef * el);
};

#endif // SVGPARSER_H
