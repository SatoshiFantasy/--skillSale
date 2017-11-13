#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "RunGuard.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    RunGuard guard( "fbsaleclient" );
        if ( !guard.tryToRun() ) {
            qCritical() << "fbsaleclient is running";
            return 0;
        }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}