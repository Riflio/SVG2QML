#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _parser = new SVGParser(this);
    _qmlGenerator = new QMLGenerator(this);
}

QUrl AppCore::generatedQMLPath() const
{
    return _generatedQMLPath;
}

/**
* @brief Парсим SVG
* @param source
* @return
*/
bool AppCore::parse(QUrl source)
{
    if ( source.isEmpty() ) { return false; }

    QFile file(source.toLocalFile());
    if ( !file.open(QIODevice::ReadOnly) ) { qWarning()<<"Unable open file"; return false; }

    qInfo()<<"Begin parse SVG"<<source.toLocalFile();

    SVGParser::ParseStatus st  = _parser->parse(&file);

    if ( st!=SVGParser::PS_OK ) { qWarning()<<"Unable parse SVG file"; return false; }

    qInfo()<<"SVG Parsed!";

    return true;
}

/**
* @brief Создаём QML
* @param dest
* @return
*/
bool AppCore::generate(QUrl dest)
{
    if ( dest.isEmpty() ) { return false; }
    qInfo()<<"Begin create QML"<<dest.toLocalFile();

    QFile fileQml(dest.toLocalFile());
    if ( !fileQml.open(QIODevice::WriteOnly|QIODevice::Text) ) { qWarning()<<"Unable open file"; return false; }

    SVGGenerator::GenerateStatus st = _qmlGenerator->generateQML(&fileQml, _parser->rootItem(), _parser->defs());

    if ( st!=SVGGenerator::GS_OK ) { qWarning()<<"Unable generate QML"; return false; }

    qInfo()<<"QML Created!";

    _generatedQMLPath = dest;
    emit generateQMLPathChanged();

    return true;
}

/**
* @brief Парсим SVG и создём из него QML
* @param source
* @param dest
*/
bool AppCore::svg2qml(QUrl source, QUrl dest)
{
    if ( !parse(source) ) { return false; }
    if ( !generate(dest) ) { return false; }

    return true;
}

