#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _parser = new SVGParser(this);
    _qmlGenerator = new QMLGenerator(this);
}

void AppCore::go()
{
    QFile file("Z:/SVG2QML/tests/test1.svg");
    if ( !file.open(QIODevice::ReadOnly) ) {
        qWarning()<<"Unable open file";
        return;
    }

    SVGParser::ParseStatus st  = _parser->parse(&file);

    if ( st!=SVGParser::PS_OK ) {
        qWarning()<<"Unable parse file";
        return;
    }

    qInfo()<<"PARSED!";

    QFile fileQml("Z:/SVG2QML/tests/test1_QML.txt");
    _qmlGenerator->generateQML(&fileQml, _parser->rootItem());

}
