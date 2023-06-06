#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlError>
#include <QUuid>
#include <QDateTime>
#include <QDate>

#include "clienthandler.h"
#include "connect_to_data_base.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    void start();
    void handleNewConnection();
    void registrationClientData(const QByteArray& data);
    void loginClientData(const QByteArray& data);
    void Create_Acc_ClientData(const QByteArray& data);


private:
    QTcpServer *tcpServer;

signals:
    void receiveLogDataFromServer(const QString& message);
    void receiveRegDataFromServer(const QString& message);
    void receiveAccDataFromServer(const QString& message);
};

#endif // SERVER_H

