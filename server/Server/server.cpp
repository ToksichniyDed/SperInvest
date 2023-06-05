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
    connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);
    connect(this, &Server::receiveLogDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения авторизации
    connect(this, &Server::receiveRegDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения регистрации


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
                }
            }
        }
    }

    QJsonObject rec;
    rec["type"]= "log";
    rec["data"]= access;

    QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
    QString message = QString::fromUtf8(byte_rec_log_data);

    emit receiveLogDataFromServer(message);
}
