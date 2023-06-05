#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readServerData);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &Client::displayError);
}

void Client::connectToServer(const QString& ipAddress, quint16 port)
{
    tcpSocket->connectToHost(ipAddress, port);
    if (!connect(tcpSocket, &QTcpSocket::connected, this, &Client::connected))
    {
        qDebug() << "Failed to connect to the server";
        return;
    }
    qDebug() << "Socket:"<<tcpSocket;
}

void Client::connected()
{
    qDebug() << "Connected to server!";
}


void Client::sendMessage(const QString& message)
{
    QByteArray data = message.toUtf8();

    if (tcpSocket && tcpSocket->isOpen())
        tcpSocket->write(data);

}

void Client::readServerData()
{
    QByteArray rec_data = tcpSocket->readAll();
    QJsonObject messageData = QJsonDocument::fromJson(rec_data).object();
    QString messageType = messageData["type"].toString();
    QJsonValue dataValue = messageData["data"];

    //Передача данных на сервер
    if (messageType == "msg")
    {
        QString msg = dataValue.toString();
        qDebug()<<"From server: "<< msg;
    }
    else if (messageType == "log")
    {
        if(dataValue == true)
            emit close_enter_window();
        else
            emit access_denied_enter_window();
    }
    else if(messageType == "reg")
    {
        QString message = dataValue.toString();
        emit rec_reg_window(message);
    }
    else
    {
        // Обработка других типов сообщений
    }

}

void Client::registrationWindowSubmit(const QJsonObject& data)
{
    // Добавляем тип данных "reg" в объект JSON
    QJsonObject messageData;
    messageData["type"] = "reg";
    messageData["data"] = data;

    // Преобразование объекта в строку JSON
    QJsonDocument jsonDoc(messageData);
    QString jsonData = jsonDoc.toJson();

    this->sendMessage(jsonData);
}

void Client::enterWindowSubmit(const QJsonObject& data)
{
    // Добавляем тип данных "log" в объект JSON
    QJsonObject messageData;
    messageData["type"] = "log";
    messageData["data"] = data;

    // Преобразование объекта в строку JSON
    QJsonDocument jsonDoc(messageData);
    QString jsonData = jsonDoc.toJson();

    this->sendMessage(jsonData);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "Error: The remote host closed the connection.";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: Host not found.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: The connection was refused by the peer.";
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "Error: Socket operation timed out.";
        break;
    default:
        qDebug() << "Error: " << tcpSocket->errorString();
        break;
    }
}




