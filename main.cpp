// #include <QCoreApplication>
#include <QApplication>
#include "src/trayicon.h"
#include "src/gatewayclient.h"
#include <QSystemTrayIcon>
#include <QDebug>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // в общем, начиная с каких то недавних версий qt решил не подтягивать шрифты с собой (даже при статической сборке)
    // ну и вынужден был заюзать данный хек
#ifdef __linux__
    // Load the embedded font.
    QString fontPath = ":/bin/Ubuntu-L.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1)
    {
        QFont font("Ubuntu-L");
        app.setFont(font);
    }
#endif

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
