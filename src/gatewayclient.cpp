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
    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

    QString status = jsonResponse.object()["rooms"].toObject()["restroom"].toObject()["status"].toString();
    if (status.indexOf("open") >= 0)
    {
        emit this->changeState(SmenaState::OPEN);
    } else {
        emit this->changeState(SmenaState::CLOSE);
    }
}

/**
 * @brief Выполняет запрос на шлюз
 */
void GatewayClient::request()
{
    // если пользователь бездействует более n секунд, то прерываем выполнение запросов
    if (ActivityAgent::getInstance()->getTimeLastActivity().addSecs(INACTIVE_AFTER_SECONDS) <= QDateTime::currentDateTime())
        return;

    // достаем имя пользователя
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString username = homePath.first().split(QDir::separator()).last();

    QUrlQuery query;
    query.addQueryItem("user", username);

    QUrl url(GATEWAY_STATUS_URL);
    url.setQuery(query);

    QNetworkRequest *request = new QNetworkRequest(url);

    this->networkManager->get(*request);
}
