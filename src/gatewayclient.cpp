#include "gatewayclient.h"

GatewayClient::GatewayClient(QObject *parent) : QObject(parent)
{
    this->timerUpdate = new QTimer();
    this->networkManager = new QNetworkAccessManager();

    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(request()));
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(updateSettings()));
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
 * @brief Обрабатывает ответ от /status/
 */
void GatewayClient::handleStatus(QNetworkReply *reply)
{
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

void GatewayClient::handleSettings(QNetworkReply *reply)
{
    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    auto settings = jsonResponse.object();
    // проверяем наличие новых версий
    static bool show_alert = false;
    if (settings["app_version"].toInt() > APP_VERSION && !show_alert)
    {
        emit this->updateAvailable();
        show_alert = true;
    }
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

    auto url = reply->url();
    auto path = url.path();
    if (path.indexOf("/settings/") == 0)
        this->handleSettings(reply);
    else if (path.indexOf("/status/") == 0)
        this->handleStatus(reply);
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

    QNetworkRequest request(url);

    this->networkManager->get(request);
}

/**
 * @brief Обновляет настройки (берет с сервера)
 */
void GatewayClient::updateSettings()
{
    auto username = this->getCurrentUserName();

    QUrlQuery query;
    query.addQueryItem("user", username);

    QUrl url(GATEWAY_SETTINGS_URL);

    url.setQuery(query);

    QNetworkRequest request(url);

    this->networkManager->get(request);
}
