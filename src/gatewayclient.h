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

#include "src/activityagent.h"

#define INACTIVE_AFTER_SECONDS 35
#define GATEWAY_STATUS_URL "http://192.168.1.100:1228/get/status/"

enum SmenaState {
    DISCONNECT,
    OPEN,
    CLOSE,
};

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
    const int TIMEOUT_BETWEEN_REQUEST = 1000; // msec

signals:
    void changeState(SmenaState state);

public slots:
    void response(QNetworkReply* reply);
    void request();
};

#endif // GATEWAYCLIENT_H
