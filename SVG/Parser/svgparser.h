#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <QObject>

#include <QFile>
#include <QXmlStreamReader>

#include "CSS/cssparser.h"

#include "Algebra/cmatrix.h"
#include "Algebra/cpoint.h"

#include "SVG/Classes/cdoc.h"
#include "SVG/Classes/cbezier.h"
#include "SVG/Classes/cline.h"
#include "SVG/Classes/cpath.h"
#include "SVG/Classes/cgroup.h"
#include "SVG/Classes/cimage.h"
#include "SVG/Classes/carc.h"
#include "SVG/Classes/ccircle.h"

#include "SVG/Classes/fclippath.h"
#include "SVG/Classes/flineargradient.h"
#include "SVG/Classes/fradialgradient.h"

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

signals:

public slots:

private:
    CDoc * _rootItem;
    QXmlStreamReader * _xml;

    CPoint _globalCoords; //-- глобальные координаты
    CMatrix _globalMatrix; //-- глобальная матрица трансформаций

    CSS::CssParser * _cssParser;

    bool parseGroup(CNodeInterface **level, QXmlStreamReader * xml);
    bool parsePath(CNodeInterface * level, QXmlStreamReader * xml);
    bool parseRect(CNodeInterface * level, QXmlStreamReader * xml);
    bool parseLine(CNodeInterface * level, QXmlStreamReader * xml);
    bool parseImage(CNodeInterface * level, QXmlStreamReader * xml);
    bool parseCss(CNodeInterface * level, QXmlStreamReader * xml);
    bool parseCircle(CNodeInterface * level, QXmlStreamReader * xml);

    CMatrix parseTransform(QXmlStreamReader * xml);
    CSS::Style parseStyle(QXmlStreamReader * xml);

    bool parseClipPath(CNodeInterface ** level, QXmlStreamReader * xml);

    bool parseLinearGradient(CNodeInterface ** level, QXmlStreamReader * xml);
    bool parseRadialGradient(CNodeInterface ** level, QXmlStreamReader * xml);
    bool parseGradientStops(FGradient * gradient, QXmlStreamReader * xml);

    CDefs _defs;

    CDef * hasLink(QXmlStreamReader * xml);

    void parseBaseAttributes(CPrimitive * itm, QXmlStreamReader * xml);

};

#endif // SVGPARSER_H
