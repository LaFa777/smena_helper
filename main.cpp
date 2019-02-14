// #include <QCoreApplication>
#include <QApplication>
#include "src/trayicon.h"
#include "src/gatewayclient.h"
#include <QSystemTrayIcon>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GatewayClient *client = new GatewayClient();
    client->run();

    TrayIcon *tray = new TrayIcon();
    tray->show();

    QObject::connect(client, SIGNAL(changeState(SmenaState)), tray, SLOT(setState(SmenaState)));

    // запускаем мониторинг активности пользователя
    ActivityAgent* monitoring = ActivityAgent::getInstance();
    monitoring->start();

    return app.exec();
}
