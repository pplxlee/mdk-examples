#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mdkplayer.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<QmlMDKPlayer>("MDKPlayer", 1, 0, "MDKPlayer");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qmdkqmlplay.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    else {
        QObject *rootObject = engine.rootObjects().first();
        qDebug() << "url:" << app.arguments().last();
        rootObject->setProperty("url", app.arguments().last());
    }

    return app.exec();
}
