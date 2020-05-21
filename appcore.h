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
    Q_PROPERTY(QString generateQMLPath READ generatedQMLPath NOTIFY generateQMLPathChanged)

public:
    explicit AppCore(QObject *parent = nullptr);

    void go(QString source, QString dest);

    QString generatedQMLPath() const;

signals:
    void generateQMLPathChanged();

public slots:

private:
    SVGParser * _parser;
    QMLGenerator * _qmlGenerator;

    QString _generatedQMLPath;
};

#endif // APPCORE_H
