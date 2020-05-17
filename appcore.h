#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include "SVG/Parser/svgparser.h"
#include "qmlgenerator.h"

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
    QMLGenerator * _qmlGenerator;
};

#endif // APPCORE_H
