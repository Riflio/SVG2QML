#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <QObject>

#include <QFile>
#include <QXmlStreamReader>

#include "CSS/cssparser.h"

#include "Algebra/cmatrix.h"
#include "Algebra/cpoint.h"

#include "SVG/Classes/cbezier.h"
#include "SVG/Classes/cline.h"
#include "SVG/Classes/cpath.h"
#include "SVG/Classes/cgroup.h"
#include "SVG/Classes/cimage.h"

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

    CPrimitive * getRootItem();

signals:

public slots:

private:
    CPrimitive * _rootItem;
    QXmlStreamReader * _xml;

    CPoint _globalCoords; //-- глобальные координаты
    CMatrix _globalMatrix; //-- глобальная матрица трансформаций

    CSS::CssParser * _cssParser;

    bool parseGroup(CPrimitive * level, QString transforms);
    bool parsePath(CPrimitive * level, QXmlStreamReader * xml);
    bool parseRect(CPrimitive * level, QXmlStreamReader * xml);
    bool parseLine(CPrimitive * level, QXmlStreamReader * xml);
    bool parseImage(CPrimitive * level, QXmlStreamReader * xml);
    bool parseCss(QString styles);

    CMatrix parseTransform(QString transform);
    CSS::Style parseStyle(QXmlStreamReader * xml);


};

#endif // SVGPARSER_H
