#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QJsonValue>
#include <QCoreApplication>
#include <QHash>
#include <QTimer>
#include <queue>
#include <QQueue>

#include "account_info.h"
#include "exchange_data.h"
#include "marketdata_info.h"
#include "purchase.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    bool connectToServer(const QString& ipAddress, quint16 port);
    void sendMessage(const QString& message);//функция отправки данных на сервер
    void registrationWindowSubmit(const QJsonObject& data);//получение данных из формы регистрации
    void enterWindowSubmit(const QJsonObject& data);//получение данных из формы входа
    void Create_Acc_WindowSubmit(const QJsonObject& data);//получение данных из формы создания счета
    QHash<QString, account_info>* get_acc_hash();
    QHash<QString, exchange_data>* get_exchange_hash();
    QHash<QString, marketdata_info>* get_market_hash();
    QHash<QString, purchase>* get_purhase_hash();
    void AddBalanceWindow(const QJsonObject& data);
    void GetBalanceWindow(const QJsonObject& data);
    void queueMessage();

private slots:
    void readServerData(QByteArray rec_data);
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();

signals:
    void close_enter_window();
    void access_denied_enter_window();
    void rec_reg_window(QString messege);
    void close_create_acc_window(QString message);
    void send_to_Show_Accounts(QString* acc_id);
    void send_to_Show_Exchange(QString exch_id);
    void show_money_history(QString amount, QString transferDate,bool isDeposit, QString accountId);
    void show_purchase_history(QString secid,int lots_count, bool is_deposit,QString purchase_datetime,QString  account_id,double all_sum,double average_price);
    void clear_accounts_window();
    void clear_history_money();
    void clear_purch_history();
    void send_acc_info(QHash<QString, account_info>* account_hash);
    void send_exchange_info(QHash<QString,exchange_data>* exchange_hash);
    void send_marketdata_info(QHash<QString, marketdata_info>* marketdata_hash);
    void update_marketdata_field(marketdata_info* marketdata_info2);
    void rec_add_money_window(QString message);
    void receivePurchaseExchangeSuccess(QString message);

private:
    QQueue<QByteArray> queue;
    QTcpSocket* tcpSocket;
    QString m_user_id;
    QHash <QString,account_info> account_hash;
    QHash <QString,exchange_data> exchange_hash;
    QHash <QString,marketdata_info> marketdata_hash;
    QHash <QString, purchase> purchase_hash;
};

#endif // CLIENT_H
