#include "connect_to_MOEX_info.h"

connect_to_MOEX_info::connect_to_MOEX_info(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    networkManager1 = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &connect_to_MOEX_info::handleRequestFinished);
    connect(networkManager1, &QNetworkAccessManager::finished, this, &connect_to_MOEX_info::handleRequestFinishedUp);
}

void connect_to_MOEX_info::executeRequest(QString exch_name)
{
    QString securitiesUrl = "https://iss.moex.com/iss/engines/stock/markets/shares/boards/TQBR/securities/" + exch_name + ".json";

    QNetworkRequest request(securitiesUrl);
    networkManager->get(request);
}

void connect_to_MOEX_info::handleRequestFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (!jsonDocument.isNull())
    {
        emit requestFinished(jsonDocument);
    }
    else
    {
        emit requestError("Failed to parse JSON response");
    }
    reply->deleteLater();
}

void connect_to_MOEX_info::executeRequestUp(QString marketdata_name)
{
    QString securitiesUrl = "https://iss.moex.com/iss/engines/stock/markets/shares/boards/TQBR/securities/" + marketdata_name + ".json";

    QNetworkRequest request(securitiesUrl);
    networkManager1->get(request);
}

void connect_to_MOEX_info::handleRequestFinishedUp(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (!jsonDocument.isNull())
    {
        emit requestFinishedUp(jsonDocument);
    }
    else
    {
        emit requestErrorUp("Failed to parse JSON response");
    }
    reply->deleteLater();
}

void connect_to_MOEX_info::get_client_handler(ClientHandler* client)
{
    this->client = client;
    connect(client, &ClientHandler::get_exch_info, this, &connect_to_MOEX_info::executeRequest);
    connect(client, &ClientHandler::get_marketdata_info, this, &connect_to_MOEX_info::executeRequestUp);
}

