#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _parser = new SVGParser(this);
    _qmlGenerator = new QMLGenerator(this);
}

QString AppCore::generatedQMLPath() const
{
    return _generatedQMLPath;
}

void AppCore::go(QString source, QString dest)
{
    QFile file(source);

    if ( !file.open(QIODevice::ReadOnly) ) {
        qWarning()<<"Unable open file";
        return;
    }

    qInfo()<<"Begin parse SVG"<<source;

    SVGParser::ParseStatus st  = _parser->parse(&file);

    if ( st!=SVGParser::PS_OK ) {
        qWarning()<<"Unable parse file";
        return;
    }

    qInfo()<<"SVG Parsed!";

    qInfo()<<"Begin create QML"<<dest;

    QFile fileQml(dest);
    _qmlGenerator->generateQML(&fileQml, _parser->rootItem(), _parser->defs());

    qInfo()<<"QML Created!";

    _generatedQMLPath = dest;
    emit generateQMLPathChanged();

}

