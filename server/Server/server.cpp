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
    connect(clientHandler, &ClientHandler::sendRegDataToServer, this, &Server::registrationClientData, Qt::UniqueConnection);//Получение регистрационных данных
    connect(clientHandler,&ClientHandler::sendLogDataToServer,this,&Server::loginClientData, Qt::UniqueConnection);// Получение авторизационных данных
    connect(clientHandler,&ClientHandler::sendCreate_Acc_DataToServer,this,&Server::Create_Acc_ClientData, Qt::UniqueConnection);// Получение данных для создания счета
    connect(clientHandler,&ClientHandler::update_accounts_data,this,&Server::Send_Accounts_Data, Qt::UniqueConnection);//Обновление информации о счетах
    connect(clientHandler,&ClientHandler::update_accounts_data,this,&Server::Send_Money_History_Data, Qt::UniqueConnection);//Обновление информации о счетах
    connect(clientHandler, &ClientHandler::send_to_add_balance,this, &Server::Add_Account_Balance, Qt::UniqueConnection);// Зачислить деньги на счет
    connect(clientHandler, &ClientHandler::purchase_exchange, this, &Server::Purchase_Exchange, Qt::UniqueConnection);// Зачисляем приобритение акций на счет.
    connect(clientHandler, &ClientHandler::update_acc_purch_data, this, &Server::Send_Main_Data_To_Client, Qt::UniqueConnection);
    connect(clientHandler, &ClientHandler::update_acc_purch_data, this, &Server::Send_History_Data, Qt::UniqueConnection);


    connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater, Qt::UniqueConnection);

    // Подключаем сигналы и слоты на отправку данных
    connect(this, &Server::receiveLogDataFromServer, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка подтверждения/отклонения авторизации
    connect(this, &Server::receiveRegDataFromServer, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка подтверждения/отклонения регистрации
    connect(this, &Server::receiveAccDataFromServer, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка подтверждения/отклонения создания счета
    connect(this, &Server::send_accounts_data, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);// Отправка данных о счете
    connect(this, &Server::set_ID,clientHandler,&ClientHandler::set_Id, Qt::UniqueConnection);//Установка user_id
    connect(this, &Server::receiveAddBalanceFromServer,clientHandler,&ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка подтверждения пополнения счета
    connect(this, &Server::receivePurchaseExchangeFromServer, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка подтверждения операции с бумагами
    connect(this, &Server::send_exch_data, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка данных о ценных бумагах на счете
    connect(this, &Server::send_money_history, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка истории денежных операций
    connect(this, &Server::send_purchase_history, clientHandler, &ClientHandler::sendMessage, Qt::UniqueConnection);//Отправка истории сделок
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

    if(access)
    {
        this->Send_Main_Data_To_Client(user_id);
        this->Send_History_Data(user_id);
    }
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
        query.prepare("INSERT INTO investment_accounts (account_id, account_name,account_balance,currency,open_date,status,user_id,created_at,tariff_plan)"
                      "VALUES (:account_id, :account_name, :account_balance, :currency, :open_date, :status, :user_id, :created_at,:tariff_plan)");

        QSqlQuery query2;
        query2.prepare("INSERT INTO money_transfers_history(transfer_id, user_id, amount, transfer_date, is_deposit, account_id)"
                       "VALUES (:transfer_id, :user_id, :amount, :transfer_date, :is_deposit, :account_id)");


        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);

        query.bindValue(":account_id", uuid);
        query2.bindValue(":account_id", uuid);
        query.bindValue(":account_name", d_acc_name);
        query.bindValue(":account_balance", d_startbalance);
        query.bindValue(":currency", d_currency);
        query.bindValue(":open_date", QDate::currentDate());
        query.bindValue(":status", "Active");
        query.bindValue(":created_at", QDateTime::currentDateTime());
        query.bindValue(":user_id", d_user_id);
        query.bindValue(":tariff_plan", d_tariffplan);

        uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);

        query2.bindValue(":transfer_id",uuid);
        query2.bindValue(":user_id",d_user_id);
        query2.bindValue(":amount",d_startbalance);
        query2.bindValue(":transfer_date",QDateTime::currentDateTime());
        query2.bindValue(":is_deposit", true);

        if (query.exec() && query2.exec()) {
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

            qDebug()<<query2.lastError().text()<<query.lastError().text();

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);

            emit receiveAccDataFromServer(message);
        }
    }
}

void Server::Send_Main_Data_To_Client(const QString& user_id)
{
    this->Send_Acc_Exc_Data(user_id);
    this->Send_Accounts_Data(user_id);
}

void Server::Send_History_Data(const QString& user_id)
{
    this->Send_Money_History_Data(user_id);
    this->Send_Purchase_History_Data(user_id);
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

void Server::Add_Account_Balance(const QByteArray &data, const QString user_id, bool isDeposit)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Извлекаем значения из JSON
        QString d_account_id = jsonObj.value("account_id").toString();
        QString d_add_balance = jsonObj.value("add_balance").toString();

        // Создаем SQL-запрос для добавления данных
        QSqlQuery query;
        QSqlQuery query2;
        if (isDeposit) {
            query.prepare("UPDATE investment_accounts SET account_balance = account_balance + :add_balance WHERE account_id = :d_account_id;");
            query2.prepare("INSERT INTO money_transfers_history(transfer_id, user_id, amount, transfer_date, is_deposit, account_id)"
                           "VALUES (:transfer_id, :user_id, :amount, :transfer_date, :is_deposit, :account_id)");
            query2.bindValue(":is_deposit", true);

        } else {
            query.prepare("UPDATE investment_accounts SET account_balance = account_balance - :add_balance WHERE account_id = :d_account_id;");
            query2.prepare("INSERT INTO money_transfers_history(transfer_id, user_id, amount, transfer_date, is_deposit, account_id)"
                           "VALUES (:transfer_id, :user_id, :amount, :transfer_date, :is_deposit, :account_id)");
            query2.bindValue(":is_deposit", false);
        }
        query.bindValue(":add_balance", d_add_balance);
        query.bindValue(":d_account_id", d_account_id);

        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.mid(1, 36);

        query2.bindValue(":transfer_id",uuid);
        query2.bindValue(":user_id",user_id);
        query2.bindValue(":amount",d_add_balance);
        query2.bindValue(":transfer_date",QDateTime::currentDateTime());
        query2.bindValue(":account_id", d_account_id);

        if (query.exec() && query2.exec()) {
            qDebug() << "Data update successfully.";
            QJsonObject rec;
            rec["type"] = "add_balance";
            rec["data"] = isDeposit ? "Успешное зачисление средств!" : "Успешное списание средств!";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            emit receiveAddBalanceFromServer(message);
        } else {
            qDebug()<<query.lastError().text()<<":"<<query2.lastError().text();
            QJsonObject rec;
            rec["type"] = "add_balance";
            rec["data"] = isDeposit ? "Ошибка зачисления средств!" : "Ошибка списания средств!";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            emit receiveAddBalanceFromServer(message);
        }
    }
}

void Server::Send_Acc_Exc_Data(const QString &user_id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM purchased_shares WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if(query.exec())
    {
        QJsonArray accountArray;

        while(query.next())
        {
            QJsonObject accountObject;

            accountObject["PURCHASE_ID"]=query.value("id_purchase").toString();
            accountObject["SECID"]=query.value("secid").toString();
            accountObject["LOTS_COUNT"]=query.value("lots_count").toString();
            accountObject["AVERAGE_PRICE"]=query.value("average_price").toString();
            accountObject["PURCHASE_DATETIME"]=query.value("purchase_datetime").toString();
            accountObject["ACCOUNT_ID"] = query.value("account_id").toString();
            accountObject["LOTSIZE"]= query.value("lotsize").toString();

            accountArray.append(accountObject);
        }

        // Создаем общий объект JSON для отправки
        QJsonObject dataObject;
        dataObject["type"]="show_exch_info";
        dataObject["data"] = QJsonValue(accountArray);

        QByteArray byte_rec_log_data = QJsonDocument(dataObject).toJson();
        QString message = QString::fromUtf8(byte_rec_log_data);

        emit send_exch_data(message);
    }
}

void Server::Send_Money_History_Data(const QString &user_id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM money_transfers_history WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (query.exec()) {
        QJsonArray transferArray;

        while (query.next()) {
            QJsonObject transferObject;
            transferObject["amount"] = query.value("amount").toString();
            transferObject["transfer_date"] = query.value("transfer_date").toString();
            transferObject["is_deposit"] = query.value("is_deposit").toBool();
            transferObject["account_id"] = query.value("account_id").toString();

            transferArray.append(transferObject);
        }

        // Создаем общий объект JSON для отправки
        QJsonObject dataObject;
        dataObject["type"] = "show_transfers";
        dataObject["data"] = QJsonValue(transferArray);

        // Преобразуем объект JSON в строку
        QByteArray byte_rec_log_data = QJsonDocument(dataObject).toJson();
        QString message = QString::fromUtf8(byte_rec_log_data);


        emit send_money_history(message);
    }
    else {
        qDebug() << "Error executing SQL query: " << query.lastError().text();

        QJsonObject dataObject;
        dataObject["type"] = "show_transfers";
        dataObject["data"] = "";

        QByteArray byte_rec_log_data = QJsonDocument(dataObject).toJson();
        QString message = QString::fromUtf8(byte_rec_log_data);


        emit send_money_history(message);
    }
}

void Server::Send_Purchase_History_Data(const QString &user_id)
{

    QSqlQuery query;
    query.prepare("SELECT * FROM purchased_history WHERE user_id = :user_id");
    query.bindValue(":user_id", user_id);

    if (query.exec()) {
        QJsonArray historyArray;

        while (query.next()) {
            QJsonObject historyObject;
            historyObject["secid"] = query.value("secid").toString();
            historyObject["lots_count"] = query.value("lots_count").toInt();
            historyObject["average_price"] = query.value("average_price").toDouble();
            historyObject["purchase_datetime"] = query.value("purchase_datetime").toString();
            historyObject["account_id"] = query.value("account_id").toString();
            historyObject["all_sum"]= query.value("all_sum").toDouble();
            historyObject["is_deposit"]=query.value("is_deposit").toBool();

            historyArray.append(historyObject);
        }

        // Создаем общий объект JSON для отправки
        QJsonObject dataObject;
        dataObject["type"] = "show_history";
        dataObject["data"] = QJsonValue(historyArray);

        QByteArray byte_rec_log_data = QJsonDocument(dataObject).toJson();
        QString message = QString::fromUtf8(byte_rec_log_data);


        emit send_purchase_history(message);
    }
    else {
        qDebug() << "Error executing SQL query: " << query.lastError().text();

        QJsonObject dataObject;
        dataObject["type"] = "show_history";
        dataObject["data"] = "";

        QByteArray byte_rec_log_data = QJsonDocument(dataObject).toJson();
        QString message = QString::fromUtf8(byte_rec_log_data);


        emit send_purchase_history(message);
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

                        //qDebug() << "Column: " << columnName << ", Data: " << data;
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

                            //qDebug() << "Column: " << columnName << ", Data: " << data;
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

                        //qDebug() << "Column: " << columnName << ", Data: " << data;
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

void Server::Purchase_Exchange(const QByteArray& data, const QString user_id, bool isDeposit)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Извлекаем значения из JSON
        QString account_id = jsonObj.value("account_id").toString();
        QString count_of_lots = jsonObj.value("count_of_lots").toString();
        QString lotsize = jsonObj.value("lotsize").toString();
        QString secid = jsonObj.value("secid").toString();
        QString price = jsonObj.value("price").toString();
        QString add_balance = jsonObj.value("add_balance").toString();

        // Создаем SQL-запрос для добавления данных
        QSqlQuery query;
        if (!isDeposit) {
            query.prepare("UPDATE investment_accounts SET account_balance = account_balance + :add_balance WHERE account_id = :d_account_id;");
        } else {
            query.prepare("UPDATE investment_accounts SET account_balance = account_balance - :add_balance WHERE account_id = :d_account_id;");
        }
        query.bindValue(":add_balance", add_balance);
        query.bindValue(":d_account_id", account_id);

        if (query.exec()) {
            qDebug() << "Acc Data update successfully.";
        }


        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM purchased_shares WHERE secid = :secid AND account_id = :account_id");
        checkQuery.bindValue(":secid", secid);
        checkQuery.bindValue(":account_id", account_id);
        if (!checkQuery.exec()) {
            // Обработка ошибки запроса
            QJsonObject rec;
            rec["type"] = "rec_purch";
            rec["data"] = "Ошибка при выполнении запроса проверки наличия бумаги.";
            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            emit receivePurchaseExchangeFromServer(message);
            qDebug() << "Ошибка при выполнении запроса проверки наличия бумаги:" << checkQuery.lastError().text();
            return;
        }

        if (checkQuery.next()) {
            int rowCount = checkQuery.value(0).toInt();
            if (rowCount == 0) {
                // Бумаги с указанным secid не существует, создаем новую запись
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO purchased_shares (id_purchase,secid, lots_count, lotsize, average_price, purchase_datetime, account_id, user_id) "
                                    "VALUES (:id_purchase, :secid, :lots_count,:lotsize, :average_price,:purchase_datetime, :account_id, :user_id)");
                QString uuid = QUuid::createUuid().toString();
                uuid = uuid.mid(1, 36);

                insertQuery.bindValue((":id_purchase"), uuid);
                insertQuery.bindValue(":secid", secid);
                insertQuery.bindValue(":lots_count", count_of_lots);
                insertQuery.bindValue(":lotsize", lotsize);
                insertQuery.bindValue(":average_price", price);
                insertQuery.bindValue(":purchase_datetime", QDateTime::currentDateTime());
                insertQuery.bindValue(":account_id", account_id);
                insertQuery.bindValue(":user_id", user_id);
                if (!insertQuery.exec()) {
                    // Обработка ошибки запроса
                    QJsonObject rec;
                    rec["type"] = "rec_purch";
                    rec["data"] = "Ошибка при создании новой записи бумаги.";
                    QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
                    QString message = QString::fromUtf8(byte_rec_log_data);
                    emit receivePurchaseExchangeFromServer(message);
                    qDebug() << "Ошибка при выполнении запроса создания новой записи бумаги:" << insertQuery.lastError().text();
                    return;
                }
            } else {
                // Бумага с указанным secid уже существует, обновляем счет
                QSqlQuery updateQuery;
                QSqlQuery selectQuery;
                selectQuery.prepare("SELECT lots_count, average_price FROM purchased_shares WHERE secid = :secid AND account_id = :account_id");
                selectQuery.bindValue(":secid", secid);
                selectQuery.bindValue(":account_id", account_id);
                if (selectQuery.exec() && selectQuery.next()) {
                    double lotsCount = selectQuery.value("lots_count").toDouble();
                    double averagePrice = selectQuery.value("average_price").toDouble();

                    if (isDeposit) {
                        lotsCount += count_of_lots.toDouble();
                        averagePrice = ((averagePrice * lotsCount) + (price.toDouble() * count_of_lots.toDouble())) / (lotsCount + count_of_lots.toDouble());
                    } else {
                        if (lotsCount - count_of_lots.toDouble() == 0.0) {
                            averagePrice = 0.0;
                        } else {
                            averagePrice = ((averagePrice * lotsCount) - (price.toDouble() * count_of_lots.toDouble())) / (lotsCount - count_of_lots.toDouble());
                        }
                        lotsCount -= count_of_lots.toDouble();
                    }

                    updateQuery.prepare("UPDATE purchased_shares SET lots_count = :new_lots_count, average_price = :new_average_price "
                                        "WHERE secid = :secid AND account_id = :account_id");

                    updateQuery.bindValue(":new_lots_count", lotsCount);
                    updateQuery.bindValue(":new_average_price", averagePrice);
                    updateQuery.bindValue(":secid", secid);
                    updateQuery.bindValue(":account_id", account_id);
                    qDebug() << "SQL Query:" << updateQuery.lastQuery();

                    if (!updateQuery.exec()) {
                        // Обработка ошибки запроса
                        QJsonObject rec;
                        rec["type"] = "rec_purch";
                        rec["data"] = isDeposit ? "Ошибка при покупке!" : "Ошибка при продаже!";
                        QJsonDocument jsonDoc(rec);
                        QString jsonData = jsonDoc.toJson();
                        emit receivePurchaseExchangeFromServer(jsonData);
                        return;
                    }
                }
            }

            QSqlQuery inserthQuery;
            inserthQuery.prepare("INSERT INTO purchased_history (id_purchase,secid, lots_count, average_price, purchase_datetime, account_id, user_id, all_sum, is_deposit) "
                                 "VALUES (:id_purchase, :secid, :lots_count, :average_price,:purchase_datetime, :account_id, :user_id, :all_sum, :is_deposit)");
            QString uuid = QUuid::createUuid().toString();
            uuid = uuid.mid(1, 36);

            inserthQuery.bindValue((":id_purchase"), uuid);
            inserthQuery.bindValue(":secid", secid);
            inserthQuery.bindValue(":lots_count", count_of_lots);
            inserthQuery.bindValue(":average_price", price);
            inserthQuery.bindValue(":purchase_datetime", QDateTime::currentDateTime());
            inserthQuery.bindValue(":account_id", account_id);
            inserthQuery.bindValue(":user_id", user_id);
            inserthQuery.bindValue(":all_sum", add_balance);
            inserthQuery.bindValue(":is_deposit", isDeposit);

            if (!inserthQuery.exec()) {
                // Обработка ошибки запроса
                QJsonObject rec;
                rec["type"] = "rec_purch";
                rec["data"] = "Ошибка при создании новой записи бумаги.";
                QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
                QString message = QString::fromUtf8(byte_rec_log_data);
                emit receivePurchaseExchangeFromServer(message);
                qDebug() << "Ошибка при выполнении запроса создания истории покупок:" << inserthQuery.lastError().text();
            }


            // Успешное выполнение запроса
            QJsonObject rec;
            rec["type"] = "rec_purch";
            rec["data"] = isDeposit ? "Успешная покупка!" : "Успешная продажа!";

            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);
            emit receivePurchaseExchangeFromServer(message);
        } else {
            // Обработка случая, когда результат запроса пустой
            QJsonObject rec;
            rec["type"] = "rec_purch";
            rec["data"] = "Ошибка при выполнении запроса проверки наличия бумаги.";
            QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
            QString message = QString::fromUtf8(byte_rec_log_data);

            emit receivePurchaseExchangeFromServer(message);

            qDebug() << "Ошибка при выполнении запроса проверки наличия бумаги: результат запроса пустой";
        }
    }
}

