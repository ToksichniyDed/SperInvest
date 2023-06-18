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
    queue_s.enqueue(message);
    qDebug()<<message;

    while (!queue_s.isEmpty()) {
        if (clientSocket && clientSocket->isOpen())
        {
            clientSocket->write(queue_s.dequeue().toUtf8());
            clientSocket->flush(); // Принудительный сброс буфера записи
        }
    }
}


void ClientHandler::readClientData()
{
    QByteArray message = clientSocket->readAll();
    queue.enqueue(message);

    while (!queue.isEmpty()) {
        QByteArray rec_data = queue.dequeue();
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
            QJsonValue dataValue = messageData["data"];

            if (dataValue.isObject())
            {
                QJsonObject dataObject = dataValue.toObject();

                // Создаем новый объект JSON для объединения user_id и data
                QJsonObject combinedData;
                combinedData["user_id"] = m_user_id;
                combinedData["data"] = dataObject;

                // Преобразуем объединенные данные в QByteArray
                QJsonDocument jsonDoc(combinedData);
                QByteArray jsonData = jsonDoc.toJson();

                emit sendCreate_Acc_DataToServer(jsonData);
            }
        }
        else if(messageType == "update_acc")
        {
            emit update_accounts_data(m_user_id);
        }
        else if(messageType == "update_purch")
        {
            emit update_acc_purch_data(m_user_id);
        }
        else if(messageType == "add_balance")
        {
            QJsonValue dataValue = messageData["data"];
            if (dataValue.isObject())
            {
                QJsonObject dataObject = dataValue.toObject();
                QByteArray jsonData = QJsonDocument(dataObject).toJson();
                emit send_to_add_balance(jsonData,m_user_id,true);
            }
        }
        else if(messageType == "get_balance")
        {
            QJsonValue dataValue = messageData["data"];
            if (dataValue.isObject())
            {
                QJsonObject dataObject = dataValue.toObject();
                QByteArray jsonData = QJsonDocument(dataObject).toJson();
                emit send_to_add_balance(jsonData,m_user_id,false);
            }
        }
        else if(messageType == "get_exchange")
        {
            QString exch_name = messageData["data"].toString();

            emit get_exch_info(exch_name);
        }
        else if(messageType == "update_marketdata")
        {
            QString marketdata_name = messageData["data"].toString();

            emit get_marketdata_info(marketdata_name);
        }
        else if(messageType == "buy_exchange"){
            QString acc_id = messageData["acc_id"].toString();
            QString result_sum = messageData["result_sum"].toString();
            QString count_lots = messageData["count_of_lots"].toString();
            QString secid = messageData["secid"].toString();
            QString price = messageData["price"].toString();
            QString lotsize = messageData["lotssize"].toString();

            QJsonObject messageData1;
            messageData1["account_id"]= acc_id;
            messageData1["add_balance"] = result_sum;
            messageData1["count_of_lots"] = count_lots;
            messageData1["secid"] = secid;
            messageData1["price"] = price;
            messageData1["lotsize"] = lotsize;

            QJsonDocument jsonDoc(messageData1);
            QByteArray jsonData = QJsonDocument(jsonDoc).toJson();

            emit purchase_exchange(jsonData,m_user_id,true);
        }
        else
        {
            //this->sendMessage("Data delivered succesfully!");
        }
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

void ClientHandler::set_Id(const QString &user_id)
{
    m_user_id = user_id;
}

