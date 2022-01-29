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
    Q_PROPERTY(QUrl generateQMLPath READ generatedQMLPath NOTIFY generateQMLPathChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)

public:
    explicit AppCore(QObject *parent = nullptr);

    QUrl generatedQMLPath() const;

    QString version() const { return "1.1"; }

signals:
    void generateQMLPathChanged();
    void versionChanged();

public slots:
    bool parse(QUrl source);
    bool generate(QUrl dest);
    bool svg2qml(QUrl source, QUrl dest);

private:
    SVGParser * _parser;
    QMLGenerator * _qmlGenerator;

    QUrl _generatedQMLPath;

};

#endif // APPCORE_H
