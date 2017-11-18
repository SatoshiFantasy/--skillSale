#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "RunGuard.h"
#include <QDebug>
#include <CoinSale.h>
#include <QCommandLineParser>
#include <QQmlContext>

using namespace fantasybit;

int main(int argc, char *argv[])
{
    RunGuard guard( "fbsaleclient" );
    if ( !guard.tryToRun() ) {
        qCritical() << "fbsaleclient is running";
        return 0;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);


    QCommandLineParser parser;
    parser.setApplicationDescription("FB Token Sale");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);

    QCommandLineOption initFileOption(QStringList() << "i" << "init",
            QCoreApplication::translate("main", "Init from file  [default: off]."));
    parser.addOption(initFileOption);

    QCommandLineOption lnetOption(QStringList() << "ln" << "lnet", "localnet");
    parser.addOption(lnetOption);

    QCommandLineOption lhostOption(QStringList() << "lh" << "lhost", "localhost");
    parser.addOption(lhostOption);

    QCommandLineOption portOpt(QStringList() << "p" << "port", "port", "port");
    parser.addOption(portOpt);

    parser.process(app);


    std::string hosturl = FBSALE_HOST;
    if ( parser.isSet(lnetOption) ) {
        hosturl = FBSALE_LOCALNET;
    }

    if ( parser.isSet(lhostOption) ) {
        hosturl = FBSALE_LOCALHOST;
    }

    int port = FBSALE_AGENT_PORT;
    if ( parser.isSet(portOpt) ) {
        port = parser.value(portOpt).toInt();
    }



    CoinSale cs(hosturl, port);


    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("CoinSale", &cs);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;





    return app.exec();
}
