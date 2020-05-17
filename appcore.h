#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>


#include "SVG/Parser/svgparser.h"

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);

    void go();

signals:

public slots:

private:
    SVGParser * _parser;
};

#endif // APPCORE_H
