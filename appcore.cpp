#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _parser = new SVGParser(this);
    _qmlGenerator = new QMLGenerator(this);
}

void AppCore::go()
{

    #ifdef WIN32
        QFile file("Z:/SVG2QML/tests/test1.svg");
    #else
        QFile file("/home/pavelk/Projects/SVG2QML/SVG2QML/tests/test1.svg");
    #endif

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

    #ifdef WIN32
        QFile fileQml("Z:/SVG2QML/tests/test1_QML.txt");
    #else
        QFile fileQml("/home/pavelk/Projects/SVG2QML/SVG2QML/tests/test1.qml");
    #endif


    _qmlGenerator->generateQML(&fileQml, _parser->rootItem());

}
