#include "gatewayclient.h"

GatewayClient::GatewayClient(QObject *parent) : QObject(parent)
{
    this->timerUpdate = new QTimer();
    this->networkManager = new QNetworkAccessManager();

    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(request()));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(response(QNetworkReply*)));
}

/**
 * @brief Запускает периодический опрос сервера
 */
void GatewayClient::run()
{
    // запускает периодический опрос шлюза на предмет открытости двери в туалете. Да. Странно звучит.
    this->timerUpdate->start(TIMEOUT_BETWEEN_REQUEST);
}

/**
 * @brief Останавливает опрос сервера
 */
void GatewayClient::stop()
{
    this->timerUpdate->stop();
}

/**
 * @brief callback для ответа сервера
 * @param reply
 */
void GatewayClient::response(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit this->changeState(SmenaState::DISCONNECT);
        return;
    }

    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

    QString status;
    auto rooms = jsonResponse.object()["rooms"].toArray();
    foreach (auto json_room, rooms) {
        auto room = json_room.toObject();
        if (QString::compare(room["name"].toString(), "restroom", Qt::CaseInsensitive) == 0)
        {
              status = room["status"].toString();
        }
    }
    if (status.indexOf("open") >= 0)
    {
        emit this->changeState(SmenaState::OPEN);
    } else {
        emit this->changeState(SmenaState::CLOSE);
    }
}

/**
 * @brief Формирует уникальное имя пользователя
 */
QString GatewayClient::getCurrentUserName()
{
    // достаем имя пользователя
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString username = homePath.first().split(QDir::separator()).last();

    QString host_name = QSysInfo::machineHostName();

    username = username + "." + host_name;

    return username;
}

/**
 * @brief Выполняет запрос на шлюз
 */
void GatewayClient::request()
{
    // если пользователь бездействует более n секунд, то прерываем выполнение запросов
    if (ActivityAgent::getInstance()->getTimeLastActivity().addSecs(INACTIVE_AFTER_SECONDS) <= QDateTime::currentDateTime())
    {
        emit this->changeState(SmenaState::DISCONNECT);
        return;
    }

    auto username = this->getCurrentUserName();

    QUrlQuery query;
    query.addQueryItem("user", username);

    QUrl url(GATEWAY_STATUS_URL);
    url.setQuery(query);

    QNetworkRequest *request = new QNetworkRequest(url);

    this->networkManager->get(*request);
}
