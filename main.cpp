#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QCommandLineParser>

#include "appcore.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    /*
    QSurfaceFormat format;
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);
    */

    QCommandLineParser parser;
    parser.setApplicationDescription("SVG to QML converter.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source svg file."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination qml file."));

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    QString source = "";
    QString dest = "";

    if ( args.count()>1 ) {
        source = args[0];
        dest = args[1];
    } else {
        #ifdef Q_OS_WIN
            source = "Z:/SVG2QML/tests/test_bezier1.svg";
            dest = "Z:/SVG2QML/tests/test1_QML.qml";
        #else
            source = "/home/pavelk/Projects/SVG2QML/SVG2QML/examples/buttons1.svg";
            dest = "/home/pavelk/Projects/SVG2QML/SVG2QML/examples/buttons1.qml";
        #endif
    }

    AppCore * appCore = new AppCore(nullptr);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCore", QVariant::fromValue(appCore));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //-- GO GO GO
    appCore->go(source, dest);


    return app.exec();
}
