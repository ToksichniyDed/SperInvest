#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonValue>

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

private slots:
    void readServerData();
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();

signals:
    void close_enter_window();
    void access_denied_enter_window();
    void rec_reg_window(QString messege);
    void close_create_acc_window(QString message);

private:
    QTcpSocket* tcpSocket;
    QString m_user_id;

};

#endif // CLIENT_H
