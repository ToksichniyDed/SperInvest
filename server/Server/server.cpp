 #include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    // Создаем экземпляр QTcpServer
    tcpServer = new QTcpServer();
    moexConnection = new connect_to_MOEX_info();

    // Подключаем сигнал newConnection к обработчику ClientHandler
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::handleNewConnection);
    connect(moexConnection, &connect_to_MOEX_info::requestFinished, this, &Server::handleMOEXResponse);
    connect(moexConnection,&connect_to_MOEX_info::requestError, this, &Server::handleRequestError);
    connect(moexConnection, &connect_to_MOEX_info::requestFinishedUp, this, &Server::handleMOEXResponseUp);
    connect(moexConnection,&connect_to_MOEX_info::requestErrorUp, this, &Server::handleRequestErrorUp);
}

Server::~Server()
{
    delete tcpServer;
    delete moexConnection;
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

    // Передаем MOEX объект класса ClientHandler
    moexConnection->get_client_handler(clientHandler);

    this->Connections_Signals(clientHandler, clientThread);

    // Запускаем поток clientThread
    clientThread->start();
}

void Server::Connections_Signals(ClientHandler* clientHandler, QThread* clientThread)
{
    // Подключаем сигналы и слоты на получение данных
    connect(clientHandler, &ClientHandler::sendRegDataToServer, this, &Server::registrationClientData);//Получение регистрационных данных
    connect(clientHandler,&ClientHandler::sendLogDataToServer,this,&Server::loginClientData);// Получение авторизационных данных
    connect(clientHandler,&ClientHandler::sendCreate_Acc_DataToServer,this,&Server::Create_Acc_ClientData);// Получение данных для создания счета
    connect(clientHandler,&ClientHandler::update_accounts_data,this,&Server::Send_Accounts_Data);//Обновление информации о счетах
    connect(clientHandler, &ClientHandler::send_to_add_balance,this, &Server::Add_Account_Balance);// Зачислить деньги на счет

    connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);

    // Подключаем сигналы и слоты на отправку данных
    connect(this, &Server::receiveLogDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения авторизации
    connect(this, &Server::receiveRegDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения регистрации
    connect(this, &Server::receiveAccDataFromServer, clientHandler, &ClientHandler::sendMessage);//Отправка подтверждения/отклонения создания счета
    connect(this, &Server::send_accounts_data, clientHandler, &ClientHandler::sendMessage);// Отправка данных о счете
    connect(this, &Server::set_ID,clientHandler,&ClientHandler::set_Id);//Установка user_id
    connect(this, &Server::receiveAddBalanceFromServer,clientHandler,&ClientHandler::sendMessage);//Отправка подтверждения пополнения счета
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

                    // Сохраняем user_id пользователя
                    query.prepare("SELECT user_id FROM users WHERE email = :email");
                    query.bindValue(":email", d_email);
                    if (query.exec() && query.next())
                        user_id = query.value(0).toString();
                    emit set_ID(user_id);
                    this->Send_Main_Data_To_Client(user_id);
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
        query.prepare("INSERT INTO investment_accounts (account_id, account_name,account_balance,currency,open_date,status,user_id,created_at,tariff_plan)" "VALUES (:account_id, :account_name, :account_balance, :currency, :open_date, :status, :user_id, :created_at,:tariff_plan)");

        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);

        query.bindValue(":account_id", uuid);
        query.bindValue(":account_name", d_acc_name);
        query.bindValue(":account_balance", d_startbalance);
        query.bindValue(":currency", d_currency);
        query.bindValue(":open_date", QDate::currentDate());
        query.bindValue(":status", "Active");
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
        else
        {
            qDebug() << "Data inserted failed.";
            QJsonObject rec;
            rec["type"]= "acc";
            rec["data"]= "Неудачное создание счета";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);

            emit receiveAccDataFromServer(message);
        }
    }
}

void Server::Send_Main_Data_To_Client(const QString& user_id)
{
    this->Send_Accounts_Data(user_id);
}

void Server::Send_Accounts_Data(const QString& user_id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM investment_accounts WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (query.exec()) {
        QJsonArray accountArray;

        while (query.next()) {
            QJsonObject accountObject;
            accountObject["account_id"] = query.value("account_id").toString();
            accountObject["account_name"] = query.value("account_name").toString();
            accountObject["account_balance"] = query.value("account_balance").toString();
            accountObject["currency"] = query.value("currency").toString();
            accountObject["open_date"] = query.value("open_date").toString();
            accountObject["status"] = query.value("status").toString();
            accountObject["created_at"] = query.value("created_at").toString();
            accountObject["tariff_plan"] = query.value("tariff_plan").toString();

            accountArray.append(accountObject);
        }

        // Создаем общий объект JSON для отправки
        QJsonObject dataObject;
        dataObject["type"]="show_acc";
        dataObject["data"] = QJsonValue(accountArray);

        // Преобразуем объект JSON в строку
        QJsonDocument jsonDoc(dataObject);
        QString jsonData = jsonDoc.toJson();

        emit send_accounts_data(jsonData);
    }
    else {
        qDebug() << "Error executing SQL query: " << query.lastError().text();

        QJsonObject dataObject;
        dataObject["type"]="show_acc";
        dataObject["data"] = "";

        // Преобразуем объект JSON в строку
        QJsonDocument jsonDoc(dataObject);
        QString jsonData = jsonDoc.toJson();

        emit send_accounts_data(jsonData);
    }
}

void Server::Add_Account_Balance(const QByteArray &data, const QString user_id)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Извлекаем значения из JSON
        QString d_account_id = jsonObj.value("account_id").toString();
        QString d_add_balance = jsonObj.value("add_balance").toString();

        qDebug()<<d_account_id;

        // Создаем SQL-запрос для добавления данных
        QSqlQuery query;
        query.prepare("UPDATE investment_accounts SET account_balance = account_balance + :add_balance WHERE account_id = :d_account_id;");
        query.bindValue(":add_balance", d_add_balance);
        query.bindValue(":d_account_id", d_account_id);

        if (query.exec()) {
            qDebug() << "Data update successfully.";
            QJsonObject rec;
            rec["type"]= "add_balance";
            rec["data"]= "Успешное зачисление средств!";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            this->Send_Accounts_Data(user_id);
            emit receiveAddBalanceFromServer(message);          
        }
        else
        {
            QJsonObject rec;
            rec["type"]= "add_balance";
            rec["data"]= "Ошибка зачисления средств!";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            this->Send_Accounts_Data(user_id);
            emit receiveAddBalanceFromServer(message);

        }
    }
}

void Server::handleMOEXResponse(const QJsonDocument &jsonDocument)
{
    QJsonObject rootObject = jsonDocument.object();
    QJsonObject securities_array;

    if (rootObject.contains("securities") && rootObject["securities"].isObject())
    {
        QJsonObject securitiesObject = rootObject["securities"].toObject();

        if (securitiesObject.contains("columns") && securitiesObject["columns"].isArray())
        {
            QJsonArray columnsArray = securitiesObject["columns"].toArray();
            QVector<QString> columnNames;

            // Обработка массива "columns"
            for (const QJsonValue &value : columnsArray)
            {
                // Пример доступа к элементу массива
                QString columnName = value.toString();
                columnNames.append(columnName);
            }

            if (securitiesObject.contains("data") && securitiesObject["data"].isArray())
            {
                QJsonArray dataArray = securitiesObject["data"].toArray();

                // Обработка массива "data"
                for (const QJsonValue &value : dataArray)
                {
                    // Пример доступа к элементу массива
                    QJsonArray rowData = value.toArray();

                    // Проверяем, что количество элементов в rowData соответствует количеству элементов в columnNames
                    if (rowData.size() != columnNames.size())
                    {
                        qDebug() << "Data and column sizes do not match";
                        continue;
                    }

                    // Обработка данных строки с соответствующими колонками
                    for (int i = 0; i < rowData.size(); ++i)
                    {
                        // Пример доступа к элементу строки и соответствующей колонке
                        QString columnName = columnNames[i];
                        QJsonValue rowValue = rowData[i];

                        QString data;
                        if (rowValue.isDouble()) {
                            double value = rowValue.toDouble();
                            data = QString::number(value);
                        } else {
                            data = rowValue.toString();
                        }

                        qDebug() << "Column: " << columnName << ", Data: " << data;
                        securities_array[columnName]=data;
                    }
                }
            }
        }

        QJsonObject marketdata_array;

        if (rootObject.contains("marketdata") && rootObject["marketdata"].isObject())
        {
            QJsonObject marketdataObject = rootObject["marketdata"].toObject();

            if (marketdataObject.contains("columns") && marketdataObject["columns"].isArray())
            {
                QJsonArray columnsArray = marketdataObject["columns"].toArray();
                QVector<QString> columnNames;

                // Обработка массива "columns"
                for (const QJsonValue &value : columnsArray)
                {
                    // Пример доступа к элементу массива
                    QString columnName = value.toString();
                    columnNames.append(columnName);
                }

                if (marketdataObject.contains("data") && marketdataObject["data"].isArray())
                {
                    QJsonArray dataArray = marketdataObject["data"].toArray();

                    // Обработка массива "data"
                    for (const QJsonValue &value : dataArray)
                    {
                        // Пример доступа к элементу массива
                        QJsonArray rowData = value.toArray();

                        // Проверяем, что количество элементов в rowData соответствует количеству элементов в columnNames
                        if (rowData.size() != columnNames.size())
                        {
                            qDebug() << "Data and column sizes do not match";
                            continue;
                        }

                        // Обработка данных строки с соответствующими колонками
                        for (int i = 0; i < rowData.size(); ++i)
                        {
                            // Пример доступа к элементу строки и соответствующей колонке
                            QString columnName = columnNames[i];
                            QJsonValue rowValue = rowData[i];

                            QString data;
                            if (rowValue.isDouble()) {
                                double value = rowValue.toDouble();
                                data = QString::number(value);
                            } else {
                                data = rowValue.toString();
                            }

                            qDebug() << "Column: " << columnName << ", Data: " << data;
                            marketdata_array[columnName]=data;
                        }
                    }
                }
            }
        }

        QJsonObject dataObject;
        dataObject["type"] = "exchange_data";
        dataObject["securities"] = QJsonValue(securities_array);
        dataObject["marketdata"] = QJsonValue(marketdata_array);

        // Преобразуем объект JSON в строку
        QJsonDocument jsonDoc(dataObject);
        QString jsonData = jsonDoc.toJson();

        emit send_accounts_data(jsonData);
    }
    else
    {
        this->handleRequestError("Invalid JSON response");
    }
}

void Server::handleRequestError(const QString &errorMessage)
{
    // Обработка ошибки подключения
    qDebug() << "Request Error: " << errorMessage;
}

void Server::handleMOEXResponseUp(const QJsonDocument &jsonDocument)
{
    QJsonObject rootObject = jsonDocument.object();
    QJsonObject marketdata_array;

    if (rootObject.contains("marketdata") && rootObject["marketdata"].isObject())
    {
        QJsonObject marketdataObject = rootObject["marketdata"].toObject();

        if (marketdataObject.contains("columns") && marketdataObject["columns"].isArray())
        {
            QJsonArray columnsArray = marketdataObject["columns"].toArray();
            QVector<QString> columnNames;

            // Обработка массива "columns"
            for (const QJsonValue &value : columnsArray)
            {
                // Пример доступа к элементу массива
                QString columnName = value.toString();
                columnNames.append(columnName);
            }

            if (marketdataObject.contains("data") && marketdataObject["data"].isArray())
            {
                QJsonArray dataArray = marketdataObject["data"].toArray();

                // Обработка массива "data"
                for (const QJsonValue &value : dataArray)
                {
                    // Пример доступа к элементу массива
                    QJsonArray rowData = value.toArray();

                    // Проверяем, что количество элементов в rowData соответствует количеству элементов в columnNames
                    if (rowData.size() != columnNames.size())
                    {
                        qDebug() << "Data and column sizes do not match";
                        continue;
                    }

                    // Обработка данных строки с соответствующими колонками
                    for (int i = 0; i < rowData.size(); ++i)
                    {
                        // Пример доступа к элементу строки и соответствующей колонке
                        QString columnName = columnNames[i];
                        QJsonValue rowValue = rowData[i];

                        QString data;
                        if (rowValue.isDouble()) {
                            double value = rowValue.toDouble();
                            data = QString::number(value);
                        } else {
                            data = rowValue.toString();
                        }

                        qDebug() << "Column: " << columnName << ", Data: " << data;
                        marketdata_array[columnName]=data;
                    }
                }
            }
        }
    }
    QJsonObject dataObject;
    dataObject["type"] = "update_marketdata";
    dataObject["marketdata"] = QJsonValue(marketdata_array);

    // Преобразуем объект JSON в строку
    QJsonDocument jsonDoc(dataObject);
    QString jsonData = jsonDoc.toJson();

    emit send_accounts_data(jsonData);
}

void Server::handleRequestErrorUp(const QString &errorMessage)
{
    // Обработка ошибки подключения
    qDebug() << "Request Error: " << errorMessage;
}
