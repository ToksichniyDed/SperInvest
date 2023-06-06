#include "clienthandler.h"

ClientHandler::ClientHandler(QTcpSocket* clientSocket, QObject* parent)
    : QObject(parent), clientSocket(clientSocket)
{
    // Подключаем сигналы и слоты для обработки данных от клиента
    connect(clientSocket, &QTcpSocket::readyRead, this, &ClientHandler::readClientData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ClientHandler::clientDisconnected);

    this->start();
}


ClientHandler::~ClientHandler()
{
    // Отключаем соединения с сигналами и слотами
    disconnect(clientSocket, &QTcpSocket::readyRead, this, &ClientHandler::readClientData);
    disconnect(clientSocket, &QTcpSocket::disconnected, this, &ClientHandler::clientDisconnected);

    qDebug()<<"Socket closed!";
}


void ClientHandler::start()
{
    qDebug()<<"New client connected!";

    QJsonObject hello_msg;
    hello_msg["type"]="msg";
    hello_msg["data"]="Welcome to server!";

    QJsonDocument helloDoc(hello_msg);
    QString jsonData = helloDoc.toJson();
    this->sendMessage(jsonData);
}

void ClientHandler::sendMessage(const QString& message)
{
    if (clientSocket && clientSocket->isOpen())
    {
        clientSocket->write(message.toUtf8());
        clientSocket->flush();// Принудительный сброс буфера записи
    }
}

void ClientHandler::readClientData()
{
    QByteArray rec_data = clientSocket->readAll();
    QJsonObject messageData = QJsonDocument::fromJson(rec_data).object();
    QString messageType = messageData["type"].toString();

    //Передача данных на сервер
    if (messageType == "reg")
    {
        QJsonValue dataValue = messageData["data"];
        if (dataValue.isObject())
        {
            QJsonObject dataObject = dataValue.toObject();
            QByteArray jsonData = QJsonDocument(dataObject).toJson();
            emit sendRegDataToServer(jsonData);
        }
    }
    else if (messageType == "log")
    {
        QJsonValue dataValue = messageData["data"];
        if (dataValue.isObject())
        {
            QJsonObject dataObject = dataValue.toObject();
            QByteArray jsonData = QJsonDocument(dataObject).toJson();
            emit sendLogDataToServer(jsonData);
        }
    }
    else if(messageType == "acc")
    {
        QString userId = messageData["user_id"].toString();
        QJsonValue dataValue = messageData["data"];

        if (dataValue.isObject())
        {
            QJsonObject dataObject = dataValue.toObject();

            // Создаем новый объект JSON для объединения user_id и data
            QJsonObject combinedData;
            combinedData["user_id"] = userId;
            combinedData["data"] = dataObject;

            // Преобразуем объединенные данные в QByteArray
            QJsonDocument jsonDoc(combinedData);
            QByteArray jsonData = jsonDoc.toJson();

            emit sendCreate_Acc_DataToServer(jsonData);
        }
    }
    else
    {
        this->sendMessage("Data delivered succesfully!");
    }
}

void ClientHandler::clientDisconnected()
{
    qDebug() << "Client disconnected";

    if (clientSocket)
    {
        clientSocket->close();
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }

    this->deleteLater();
}

