#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QIcon>
#include <QDebug>


#include <QThread>

#include "src/activityagent.h"
#include "src/gatewayclient.h"

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent = nullptr);

private:
    QMenu *contextMenu;
    ActivityAgent *monitoring;
    SmenaState prevSmenaState;

    void initMenu();

public slots:
    void setState(SmenaState state);
    void needUpdateAlert();
};

#endif // TRAYICON_H
