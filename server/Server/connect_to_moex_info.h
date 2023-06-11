#ifndef CONNECT_TO_MOEX_INFO_H
#define CONNECT_TO_MOEX_INFO_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

#include "clienthandler.h"


class connect_to_MOEX_info: public QObject
{
    Q_OBJECT

public:
    explicit connect_to_MOEX_info(QObject *parent = nullptr);

    void executeRequest(QString exch_name);
    void executeRequestUp(QString marketdata_name);
    void get_client_handler(ClientHandler* client);

signals:
    void requestFinished(const QJsonDocument &jsonDocument);
    void requestError(const QString &errorMessage);
    void requestFinishedUp(const QJsonDocument &jsonDocument);
    void requestErrorUp(const QString &errorMessage);


private slots:
    void handleRequestFinished(QNetworkReply *reply);
    void handleRequestFinishedUp(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QNetworkAccessManager *networkManager1;
    ClientHandler* client;
};

#endif // CONNECT_TO_MOEX_INFO_H
