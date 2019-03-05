#ifndef GATEWAYCLIENT_H
#define GATEWAYCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValueRef>
#include <QSysInfo>

#include "src/activityagent.h"

#define INACTIVE_AFTER_SECONDS 35
#define GATEWAY_STATUS_URL "http://192.168.1.100:8000/status/"
#define GATEWAY_SETTINGS_URL "http://192.168.1.100:8000/settings/"

enum SmenaState {
    DISCONNECT,
    OPEN,
    CLOSE,
};

#define APP_VERSION 1

class GatewayClient : public QObject
{
    Q_OBJECT
public:
    explicit GatewayClient(QObject *parent = nullptr);
    void run();
    void stop();

private:
    QNetworkAccessManager *networkManager;
    QTimer *timerUpdate;
    QJsonObject settings;
    const int TIMEOUT_BETWEEN_REQUEST = 1000; // msec

    QString getCurrentUserName();
    void handleStatus(QNetworkReply *reply);
    void handleSettings(QNetworkReply *reply);
signals:
    void changeState(SmenaState state);
    void updateAvailable();

public slots:
    void response(QNetworkReply* reply);
    void request();
    void updateSettings();
};

#endif // GATEWAYCLIENT_H
