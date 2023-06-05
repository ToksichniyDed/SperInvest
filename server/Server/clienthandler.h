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

private:
    QTcpSocket *clientSocket;
    QThread* thread;

signals:
    void sendRegDataToServer(const QByteArray& data);
    void sendLogDataToServer(const QByteArray& data);
};

#endif // CLIENTHANDLER_H
