#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket* clientSocket = nullptr, QObject* parent = nullptr);
    ~ClientHandler();

public slots:
    void start();
    void sendMessage(const QString& message);
    void readClientData();
    void clientDisconnected();
    void set_Id(const QString& user_id);

private:
    QTcpSocket *clientSocket;
    QThread* thread;
    QString m_user_id;

signals:
    void sendRegDataToServer(const QByteArray& data);
    void sendLogDataToServer(const QByteArray& data);
    void sendCreate_Acc_DataToServer(const QByteArray& data);
    void update_accounts_data(const QString& user_id);
    void send_to_add_balance(const QByteArray& data,const QString user_id);
};

#endif // CLIENTHANDLER_H
