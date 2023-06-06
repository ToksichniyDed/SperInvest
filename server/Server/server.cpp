#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    // Создаем экземпляр QTcpServer
    tcpServer = new QTcpServer();

    // Подключаем сигнал newConnection к обработчику ClientHandler
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

Server::~Server()
{
    delete tcpServer;
}

void Server::start()
{
    if (!tcpServer->listen(QHostAddress::LocalHost, 1234)) {
        qDebug() << "Failed to start server.";
    } else {
        qDebug() << "Server started. Listening on IP:" << tcpServer->serverAddress().toString()
                 << "Port:" << tcpServer->serverPort();
    }
}

void Server::handleNewConnection()
{
    // Получаем новый сокет
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();

    // Создаем новый поток
    QThread* clientThread = new QThread;

    // Устанавливаем родительский объект как nullptr
    clientSocket->setParent(nullptr);

    // Перемещаем сокет в поток clientThread
    clientSocket->moveToThread(clientThread);

    // Создаем экземпляр класса ClientHandler без передачи сокета в конструкторе
    ClientHandler* clientHandler = new ClientHandler(clientSocket);

    // Перемещаем экземпляр ClientHandler в поток clientThread
    clientHandler->moveToThread(clientThread);

    // Подключаем сигналы и слоты
    connect(clientHandler, &ClientHandler::sendRegDataToServer, this, &Server::registrationClientData);//Получение регистрационных данных
    connect(clientHandler,&ClientHandler::sendLogDataToServer,this,&Server::loginClientData);// Получение авторизационных данных
    connect(clientHandler,&ClientHandler::sendCreate_Acc_DataToServer,this,&Server::loginClientData);// Получение данных для создания счета
    connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);
    connect(this, &Server::receiveLogDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения авторизации
    connect(this, &Server::receiveRegDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения регистрации
    connect(this, &Server::receiveAccDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения создания счета


    // Запускаем поток clientThread
    clientThread->start();

    // Вызываем метод setSocket у clientHandler в его потоке
    //QMetaObject::invokeMethod(clientHandler, "setSocket", Qt::QueuedConnection, Q_ARG(QTcpSocket*, clientSocket));
}

void Server::registrationClientData(const QByteArray& data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull()) {
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            // Извлекаем значения из JSON
            QString d_email = jsonObj.value("Email").toString();
            QString d_password = jsonObj.value("Password").toString();
            QString d_phone = jsonObj.value("Phone").toString();
            QString d_username = jsonObj.value("Username").toString();

            //Проверка на совпадение email
            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
            checkQuery.bindValue(":email", d_email);
            if (checkQuery.exec() && checkQuery.next()) {
                int count = checkQuery.value(0).toInt();
                if (count > 0) {
                    QJsonObject rec;
                    rec["type"]= "reg";
                    rec["data"]= "Пользователь с таким логином уже существует.";

                    QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
                    QString message = QString::fromUtf8(byte_rec_log_data);

                    emit receiveRegDataFromServer(message);
                }
            }

            // Создаем SQL-запрос для добавления данных
            QSqlQuery query;
            query.prepare("INSERT INTO users (user_id, username, email, password, status, phone, created_at, updated_at)" "VALUES (:user_id, :username, :email, :password, :status, :phone , :created_at, :updated_at)");

            //Генерация user_id
            QString uuid = QUuid::createUuid().toString();
            uuid = uuid.mid(1, 36);

            query.bindValue(":user_id", uuid);
            query.bindValue(":username", d_username);
            query.bindValue(":email", d_email);
            query.bindValue(":password", d_password);
            query.bindValue(":status", false);
            query.bindValue(":phone", d_phone);
            query.bindValue(":created_at", QDateTime::currentDateTime());
            query.bindValue(":updated_at", QDateTime::currentDateTime());


            if (query.exec()) {
                qDebug() << "Data inserted successfully.";
                QJsonObject rec;
                rec["type"]= "reg";
                rec["data"]= "Успешная регистрация";

                QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
                QString message = QString::fromUtf8(byte_rec_log_data);

                emit receiveRegDataFromServer(message);
            }

            else {
                qDebug() << "Failed to insert data:" << query.lastError().text();
            }
        }
    } else {
        qDebug() << "Failed to parse JSON.";
    }
}

void Server::loginClientData(const QByteArray& data)
{
    bool access = false;
    QString user_id = "null";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull()) {
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            // Извлекаем значения из JSON
            QString d_email = jsonObj.value("Email").toString();
            QString d_password = jsonObj.value("Password").toString();

            // Проверка данных пользователя с базой данных
            QSqlQuery query;
            query.prepare("SELECT COUNT(*) FROM users WHERE email = :email AND password = :password");
            query.bindValue(":email", d_email);
            query.bindValue(":password", d_password);

            if (query.exec() && query.next()) {
                int count = query.value(0).toInt();
                if (count > 0) {
                    // Пользователь существует
                    access = true;
                    query.prepare("SELECT user_id FROM users WHERE email = :email");
                    query.bindValue(":email", d_email);
                    if (query.exec() && query.next())
                        user_id = query.value(0).toString();
                }
            }
        }
    }

    QJsonObject rec;
    rec["type"]= "log";
    rec["data"]= access;
    rec["user_id"] = user_id;

    QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
    QString message = QString::fromUtf8(byte_rec_log_data);

    emit receiveLogDataFromServer(message);
}

void Server::Create_Acc_ClientData(const QByteArray& data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Извлекаем значения из JSON
        QString d_user_id = jsonObj.value("user_id").toString();

        QJsonObject jsonObj2 = jsonObj.value("data").toObject();
        QString d_acc_name = jsonObj2.value("acc_name").toString();
        QString d_currency = jsonObj2.value("currency").toString();
        QString d_startbalance = jsonObj2.value("startbalance").toString();
        QString d_tariffplan = jsonObj2.value("tariffplan").toString();

        // Создаем SQL-запрос для добавления данных
        QSqlQuery query;
        query.prepare("INSERT INTO investment_accounts (account_id, account_name,account_balance,currency,open_date,status,user_id,created_at,tariff_plan)" "VALUE (:account_id, :account_name, :account_balance, :currency, :open_date, :status, :user_id, :created_at,:tariff_plan)");

        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);

        query.bindValue(":account_id", uuid);
        query.bindValue(":account_name", d_acc_name);
        query.bindValue(":account_balance", d_startbalance);
        query.bindValue(":currency", d_currency);
        query.bindValue(":open_date", QDateTime::currentDateTime());
        query.bindValue(":status", true);
        query.bindValue(":created_at", QDateTime::currentDateTime());
        query.bindValue(":user_id", d_user_id);
        query.bindValue(":tariff_plan", d_tariffplan);

        if (query.exec()) {
            qDebug() << "Data inserted successfully.";
            QJsonObject rec;
            rec["type"]= "acc";
            rec["data"]= "Успешное создание счета";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);

            emit receiveAccDataFromServer(message);
        }
    }

}
