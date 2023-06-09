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

#include"account_info.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void connectToServer(const QString& ipAddress, quint16 port);
    void sendMessage(const QString& message);//функция отправки данных на сервер
    void registrationWindowSubmit(const QJsonObject& data);//получение данных из формы регистрации
    void enterWindowSubmit(const QJsonObject& data);//получение данных из формы входа
    void Create_Acc_WindowSubmit(const QJsonObject& data);//получение данных из формы создания счета
    QHash<QString, account_info>* get_acc_hash();

private slots:
    void readServerData();
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();

signals:
    void close_enter_window();
    void access_denied_enter_window();
    void rec_reg_window(QString messege);
    void close_create_acc_window(QString message);
    void send_to_Show_Accounts(QString* acc_id);
    void clear_accounts_window();
    void send_acc_info(QHash<QString, account_info>* account_hash);

private:
    QTcpSocket* tcpSocket;
    QString m_user_id;
    QHash <QString,account_info> account_hash;
};

#endif // CLIENT_H
