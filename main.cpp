#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QCommandLineParser>

#include "appcore.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);


    QCommandLineParser parser;
    parser.setApplicationDescription("SVG to QML converter.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source svg file."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination qml file."));

    parser.process(app);
    const QStringList args = parser.positionalArguments();

    QString source = (args.count()>0)? args[0] : "";
    QString dest = (args.count()>1)? args[1] : "file:./tmp.qml";

    AppCore * appCore = new AppCore(nullptr);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCore", QVariant::fromValue(appCore));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) { QCoreApplication::exit(-1); }
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("appCore", appCore);

    engine.load(url);

    //-- Create
    if ( !source.isEmpty() ) {
        appCore->svg2qml(source, dest);
    }

    return app.exec();
}
