#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readServerData);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &Client::displayError);
}

bool Client::connectToServer(const QString& ipAddress, quint16 port)
{
    tcpSocket->connectToHost(ipAddress, port);
    if (tcpSocket->waitForConnected(10000)) {
        qDebug() << "Successfully connected to the server";
        return true;
    } else {
        qDebug() << "Failed to connect to the server";
        return connectToServer("127.0.0.1", 1234);
    }
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
        {
            emit close_enter_window();
            m_user_id = messageData["user_id"].toString();
        }
        else
            emit access_denied_enter_window();
    }
    else if(messageType == "reg")
    {
        QString message = dataValue.toString();
        emit rec_reg_window(message);
    }
    else if(messageType == "acc")
    {
        QString message = dataValue.toString();
        emit close_create_acc_window(message);
        QJsonObject rec;
        rec["type"]= "update_acc";
        rec["data"] = "";
        QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
        QString update = QString::fromUtf8(byte_rec_log_data);
        this->sendMessage(update);
    }
    else if(messageType == "show_acc")
    {
        if (dataValue.isArray())
        {
            account_hash.clear();
            emit clear_accounts_window();
            QJsonArray accountArray = dataValue.toArray();

            for (const QJsonValue& accountValue : accountArray)
            {

                if (accountValue.isObject())
                {
                    QJsonObject accountObject = accountValue.toObject();

                    QString acc_id = accountObject["account_id"].toString();

                    account_info acc_info(accountObject["account_id"].toString(),accountObject["account_name"].toString(),accountObject["account_balance"].toString(),accountObject["currency"].toString()
                                          ,accountObject["open_date"].toString(),accountObject["status"].toString(),
                                          accountObject["created_at"].toString(),accountObject["tariff_plan"].toString());

                    account_hash.insert(acc_id,acc_info);
                }
            }
            emit send_acc_info(&account_hash);

            for(QString& key : account_hash.keys())
            {
                emit send_to_Show_Accounts(&key);
            }
        }
        else
        {
            //emit send_to_Show_Accounts(dataValue);
        }
    }
    else if(messageType == "add_balance")
    {
        QString message = dataValue.toString();
        emit rec_add_money_window(message);
    }
    else
    {
        qDebug()<<"Recieve from server!";
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

void Client::Create_Acc_WindowSubmit(const QJsonObject& data)
{
    // Добавляем тип данных "acc" в объект JSON
    QJsonObject messageData;
    messageData["type"] = "acc";
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
        QMessageBox::critical(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: Host not found.";
        QMessageBox::critical(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: The connection was refused by the peer.";
        QMessageBox::critical(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "Error: Socket operation timed out.";
        QMessageBox::critical(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    default:
        qDebug() << "Error: " << tcpSocket->errorString();
        break;
    }
}

QHash<QString, account_info>* Client::get_acc_hash()
{
    return &account_hash;
}

void Client::AddBalanceWindow(const QJsonObject &data)
{
    // Добавляем тип данных "acc" в объект JSON
    QJsonObject messageData;
    messageData["type"] = "add_balance";
    messageData["data"] = data;

    // Преобразование объекта в строку JSON
    QJsonDocument jsonDoc(messageData);
    QString jsonData = jsonDoc.toJson();

    this->sendMessage(jsonData);
}





