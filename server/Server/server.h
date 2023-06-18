#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>
#include <QUuid>
#include <QDateTime>
#include <QDate>
#include <QThread>


#include "clienthandler.h"
#include "connect_to_data_base.h"
#include "connect_to_MOEX_info.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    void start();
    void handleNewConnection();
    void Connections_Signals(ClientHandler* clientHandler,QThread* clientThread);
    void registrationClientData(const QByteArray& data);
    void loginClientData(const QByteArray& data);
    void Create_Acc_ClientData(const QByteArray& data);
    void Send_Main_Data_To_Client(const QString& user_id);
    void Send_Accounts_Data(const QString& user_id);
    void Send_History_Data(const QString& user_id);
    void Send_Acc_Exc_Data(const QString& user_id);
    void Send_Money_History_Data(const QString& user_id);
    void Send_Purchase_History_Data(const QString& user_id);
    void Add_Account_Balance(const QByteArray& data, const QString user_id,bool isDeposit);
    void Purchase_Exchange(const QByteArray& data, const QString user_id, bool isDeposit);
    void handleMOEXResponse(const QJsonDocument &jsonDocument);
    void handleRequestError(const QString &errorMessage);
    void handleMOEXResponseUp(const QJsonDocument &jsonDocument);
    void handleRequestErrorUp(const QString &errorMessage);

private:
    QTcpServer *tcpServer;
    connect_to_MOEX_info *moexConnection;

signals:
    void receiveLogDataFromServer(const QString& message);
    void receiveRegDataFromServer(const QString& message);
    void receiveAccDataFromServer(const QString& message);
    void receiveAddBalanceFromServer(const QString& message);
    void receivePurchaseExchangeFromServer(QString message);

    void send_accounts_data(const QString& message);
    void send_exch_data(const QString& message);
    void send_money_history(const QString& message);
    void send_purchase_history(const QString& message);
    void set_ID(const QString& user_id);
};

#endif // SERVER_H

