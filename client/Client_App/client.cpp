#include "client.h"
#include <QThread>

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::queueMessage);
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

void Client::queueMessage()
{
    QByteArray message = tcpSocket->readAll();
    //qDebug()<<message;
    queue.enqueue(message);
    QTimer::singleShot(50, std::bind(&Client::readServerData, this, queue.dequeue()));
}

void Client::readServerData(QByteArray rec_data)
{   
    QJsonObject messageData = QJsonDocument::fromJson(rec_data).object();
    QString messageType = messageData["type"].toString();
    QJsonValue dataValue = messageData["data"];

    qDebug()<<messageType;

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
    }
    else if(messageType == "show_acc")
    {
        if (dataValue.isArray())
        {
            account_hash.clear();
            emit clear_accounts_window();
            emit clear_history_money();
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
            qDebug()<<"Ошибка показывания счетов!";
        }
    }
    else if(messageType == "add_balance")
    {
        QString message = dataValue.toString();

        emit rec_add_money_window(message);

        emit close_create_acc_window(message);
        QJsonObject rec;
        rec["type"]= "update_acc";
        rec["data"] = "";
        QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
        QString update = QString::fromUtf8(byte_rec_log_data);

        this->sendMessage(update);
    }
    else if (messageType == "exchange_data")
    {
        QJsonObject securitiesObject = messageData["securities"].toObject();
        QJsonObject marketdataObject = messageData["marketdata"].toObject();

        if (!securitiesObject.isEmpty() && !marketdataObject.isEmpty())
        {
            QString exchange_id = securitiesObject["SECID"].toString();

            if (!exchange_hash.contains(exchange_id))
            {
                exchange_data exch_data(
                    securitiesObject["SECID"].toString(),
                    securitiesObject["BOARDID"].toString(),
                    securitiesObject["SHORTNAME"].toString(),
                    securitiesObject["PREVPRICE"].toString(),
                    securitiesObject["LOTSIZE"].toString(),
                    securitiesObject["FACEVALUE"].toString(),
                    securitiesObject["STATUS"].toString(),
                    securitiesObject["BOARDNAME"].toString(),
                    securitiesObject["DECIMALS"].toString(),
                    securitiesObject["SECNAME"].toString(),
                    securitiesObject["REMARKS"].toString(),
                    securitiesObject["MARKETCODE"].toString(),
                    securitiesObject["INSTRID"].toString(),
                    securitiesObject["SECTORID"].toString(),
                    securitiesObject["MINSTEP"].toString(),
                    securitiesObject["PREVWAPRICE"].toString(),
                    securitiesObject["FACEUNIT"].toString(),
                    securitiesObject["PREVDATE"].toString(),
                    securitiesObject["ISSUESIZE"].toString(),
                    securitiesObject["ISIN"].toString(),
                    securitiesObject["LATNAME"].toString(),
                    securitiesObject["REGNUMBER"].toString(),
                    securitiesObject["PREVLEGALCLOSEPRICE"].toString(),
                    securitiesObject["CURRENCYID"].toString(),
                    securitiesObject["SECTYPE"].toString(),
                    securitiesObject["LISTLEVEL"].toString(),
                    securitiesObject["SETTLEDATE"].toString()
                    );

                exchange_hash.insert(exchange_id, exch_data);
                emit send_exchange_info(&exchange_hash);
            }
            else
            {
                emit send_to_Show_Exchange(exchange_id);
            }

            QString marketdata_id = marketdataObject["SECID"].toString();

            if (!marketdata_hash.contains(marketdata_id))
            {
                marketdata_info marketdata_data(
                    marketdataObject["SECID"].toString(),
                    marketdataObject["BOARDID"].toString(),
                    marketdataObject["BID"].toString(),
                    marketdataObject["BIDDEPTH"].toString(),
                    marketdataObject["OFFER"].toString(),
                    marketdataObject["OFFERDEPTH"].toString(),
                    marketdataObject["SPREAD"].toString(),
                    marketdataObject["BIDDEPTHT"].toString(),
                    marketdataObject["OFFERDEPTHT"].toString(),
                    marketdataObject["OPEN"].toString(),
                    marketdataObject["LOW"].toString(),
                    marketdataObject["HIGH"].toString(),
                    marketdataObject["LAST"].toString(),
                    marketdataObject["LASTCHANGE"].toString(),
                    marketdataObject["LASTCHANGEPRCNT"].toString(),
                    marketdataObject["QTY"].toString(),
                    marketdataObject["VALUE"].toString(),
                    marketdataObject["VALUE_USD"].toString(),
                    marketdataObject["WAPRICE"].toString(),
                    marketdataObject["LASTCNGTOLASTWAPRICE"].toString(),
                    marketdataObject["WAPTOPREVWAPRICEPRCNT"].toString(),
                    marketdataObject["WAPTOPREVWAPRICE"].toString(),
                    marketdataObject["CLOSEPRICE"].toString(),
                    marketdataObject["MARKETPRICETODAY"].toString(),
                    marketdataObject["MARKETPRICE"].toString(),
                    marketdataObject["LASTTOPREVPRICE"].toString(),
                    marketdataObject["NUMTRADES"].toString(),
                    marketdataObject["VOLTODAY"].toString(),
                    marketdataObject["VALTODAY"].toString(),
                    marketdataObject["VALTODAY_USD"].toString(),
                    marketdataObject["ETFSETTLEPRICE"].toString(),
                    marketdataObject["TRADINGSTATUS"].toString(),
                    marketdataObject["UPDATETIME"].toString(),
                    marketdataObject["LASTBID"].toString(),
                    marketdataObject["LASTOFFER"].toString(),
                    marketdataObject["LCLOSEPRICE"].toString(),
                    marketdataObject["LCURRENTPRICE"].toString(),
                    marketdataObject["MARKETPRICE2"].toString(),
                    marketdataObject["NUMBIDS"].toString(),
                    marketdataObject["NUMOFFERS"].toString(),
                    marketdataObject["CHANGE"].toString(),
                    marketdataObject["TIME"].toString(),
                    marketdataObject["HIGHBID"].toString(),
                    marketdataObject["LOWOFFER"].toString(),
                    marketdataObject["PRICEMINUSPREVWAPRICE"].toString(),
                    marketdataObject["OPENPERIODPRICE"].toString(),
                    marketdataObject["SEQNUM"].toString(),
                    marketdataObject["SYSTIME"].toString(),
                    marketdataObject["CLOSINGAUCTIONPRICE"].toString(),
                    marketdataObject["CLOSINGAUCTIONVOLUME"].toString(),
                    marketdataObject["ISSUECAPITALIZATION"].toString(),
                    marketdataObject["ISSUECAPITALIZATION_UPDATETIME"].toString(),
                    marketdataObject["ETFSETTLECURRENCY"].toString(),
                    marketdataObject["VALTODAY_RUR"].toString(),
                    marketdataObject["TRADINGSESSION"].toString()
                    );

                marketdata_hash.insert(marketdata_id, marketdata_data);
                emit send_marketdata_info(&marketdata_hash);
            }
            emit send_to_Show_Exchange(exchange_id);
        }
    }
    else if(messageType == "update_marketdata")
    {
        QJsonObject marketdataObject = messageData["marketdata"].toObject();

        if (!marketdataObject.isEmpty())
        {
            QString marketdata_id = marketdataObject["SECID"].toString();

            marketdata_info& marketdata_data = marketdata_hash[marketdata_id];

            marketdata_data.setSECID(marketdataObject["SECID"].toString());
            marketdata_data.setBOARDID(marketdataObject["BOARDID"].toString());
            marketdata_data.setBID(marketdataObject["BID"].toString());
            marketdata_data.setBIDDEPTH(marketdataObject["BIDDEPTH"].toString());
            marketdata_data.setOFFER(marketdataObject["OFFER"].toString());
            marketdata_data.setOFFERDEPTH(marketdataObject["OFFERDEPTH"].toString());
            marketdata_data.setSPREAD(marketdataObject["SPREAD"].toString());
            marketdata_data.setBIDDEPTHT(marketdataObject["BIDDEPTHT"].toString());
            marketdata_data.setOFFERDEPTHT(marketdataObject["OFFERDEPTHT"].toString());
            marketdata_data.setOPEN(marketdataObject["OPEN"].toString());
            marketdata_data.setLOW(marketdataObject["LOW"].toString());
            marketdata_data.setHIGH(marketdataObject["HIGH"].toString());
            marketdata_data.setLAST(marketdataObject["LAST"].toString());
            marketdata_data.setLASTCHANGE(marketdataObject["LASTCHANGE"].toString());
            marketdata_data.setLASTCHANGEPRCNT(marketdataObject["LASTCHANGEPRCNT"].toString());
            marketdata_data.setQTY(marketdataObject["QTY"].toString());
            marketdata_data.setVALUE(marketdataObject["VALUE"].toString());
            marketdata_data.setVALUE_USD(marketdataObject["VALUE_USD"].toString());
            marketdata_data.setWAPRICE(marketdataObject["WAPRICE"].toString());
            marketdata_data.setLASTCNGTOLASTWAPRICE(marketdataObject["LASTCNGTOLASTWAPRICE"].toString());
            marketdata_data.setWAPTOPREVWAPRICEPRCNT(marketdataObject["WAPTOPREVWAPRICEPRCNT"].toString());
            marketdata_data.setWAPTOPREVWAPRICE(marketdataObject["WAPTOPREVWAPRICE"].toString());
            marketdata_data.setCLOSEPRICE(marketdataObject["CLOSEPRICE"].toString());
            marketdata_data.setMARKETPRICETODAY(marketdataObject["MARKETPRICETODAY"].toString());
            marketdata_data.setMARKETPRICE(marketdataObject["MARKETPRICE"].toString());
            marketdata_data.setLASTTOPREVPRICE(marketdataObject["LASTTOPREVPRICE"].toString());
            marketdata_data.setNUMTRADES(marketdataObject["NUMTRADES"].toString());
            marketdata_data.setVOLTODAY(marketdataObject["VOLTODAY"].toString());
            marketdata_data.setVALTODAY(marketdataObject["VALTODAY"].toString());
            marketdata_data.setVALTODAY_USD(marketdataObject["VALTODAY_USD"].toString());
            marketdata_data.setETFSETTLEPRICE(marketdataObject["ETFSETTLEPRICE"].toString());
            marketdata_data.setTRADINGSTATUS(marketdataObject["TRADINGSTATUS"].toString());
            marketdata_data.setUPDATETIME(marketdataObject["UPDATETIME"].toString());
            marketdata_data.setLASTBID(marketdataObject["LASTBID"].toString());
            marketdata_data.setLASTOFFER(marketdataObject["LASTOFFER"].toString());
            marketdata_data.setLCLOSEPRICE(marketdataObject["LCLOSEPRICE"].toString());
            marketdata_data.setLCURRENTPRICE(marketdataObject["LCURRENTPRICE"].toString());
            marketdata_data.setMARKETPRICE2(marketdataObject["MARKETPRICE2"].toString());
            marketdata_data.setNUMBIDS(marketdataObject["NUMBIDS"].toString());
            marketdata_data.setNUMOFFERS(marketdataObject["NUMOFFERS"].toString());
            marketdata_data.setCHANGE(marketdataObject["CHANGE"].toString());
            marketdata_data.setTIME(marketdataObject["TIME"].toString());
            marketdata_data.setHIGHBID(marketdataObject["HIGHBID"].toString());
            marketdata_data.setLOWOFFER(marketdataObject["LOWOFFER"].toString());
            marketdata_data.setPRICEMINUSPREVWAPRICE(marketdataObject["PRICEMINUSPREVWAPRICE"].toString());
            marketdata_data.setOPENPERIODPRICE(marketdataObject["OPENPERIODPRICE"].toString());
            marketdata_data.setSEQNUM(marketdataObject["SEQNUM"].toString());
            marketdata_data.setSYSTIME(marketdataObject["SYSTIME"].toString());
            marketdata_data.setCLOSINGAUCTIONPRICE(marketdataObject["CLOSINGAUCTIONPRICE"].toString());
            marketdata_data.setCLOSINGAUCTIONVOLUME(marketdataObject["CLOSINGAUCTIONVOLUME"].toString());
            marketdata_data.setISSUECAPITALIZATION(marketdataObject["ISSUECAPITALIZATION"].toString());
            marketdata_data.setISSUECAPITALIZATION_UPDATETIME(marketdataObject["ISSUECAPITALIZATION_UPDATETIME"].toString());
            marketdata_data.setETFSETTLECURRENCY(marketdataObject["ETFSETTLECURRENCY"].toString());
            marketdata_data.setVALTODAY_RUR(marketdataObject["VALTODAY_RUR"].toString());
            marketdata_data.setTRADINGSESSION(marketdataObject["TRADINGSESSION"].toString());

            emit update_marketdata_field(&marketdata_data);
        }
    }
    else if (messageType == "rec_purch") {
        QString message = messageData["data"].toString();
        emit receivePurchaseExchangeSuccess(message);
    }
    else if(messageType == "show_exch_info")
    {
        if (dataValue.isArray())
        {
            purchase_hash.clear();
            emit clear_purch_history();
            QJsonArray purchaseArray = dataValue.toArray();

            for (const QJsonValue& purchaseValue : purchaseArray)
            {

                if (purchaseValue.isObject())
                {
                    QJsonObject purchaseObject = purchaseValue.toObject();

                    QString purchase_id = purchaseObject["PURCHASE_ID"].toString();

                    purchase purch (purchaseObject["PURCHASE_ID"].toString(),purchaseObject["SECID"].toString(),purchaseObject["LOTS_COUNT"].toString(),purchaseObject["LOTSIZE"].toString(),
                                   purchaseObject["AVERAGE_PRICE"].toString(),purchaseObject["PURCHASE_DATETIME"].toString(),purchaseObject["ACCOUNT_ID"].toString());

                    purchase_hash.insert(purchase_id,purch);
                }
            }
        }
    }
    else if(messageType == "show_transfers")
    {
        if (dataValue.isArray()){
            QJsonArray transferArray = dataValue.toArray();

            for (int i = 0; i < transferArray.size(); ++i) {
                QJsonObject transferObj = transferArray[i].toObject();

                QString amount = transferObj["amount"].toString();
                QString transferDate = transferObj["transfer_date"].toString();
                bool isDeposit = transferObj["is_deposit"].toBool();
                QString accountId = transferObj["account_id"].toString();

                emit show_money_history (amount, transferDate, isDeposit, accountId);
            }
        }
    }
    else if(messageType == "show_history")
    {
        if (dataValue.isArray()){
            QJsonArray exch_buyArray = dataValue.toArray();

            for (int i = 0; i < exch_buyArray.size(); ++i) {
                QJsonObject exch_buyObj = exch_buyArray[i].toObject();

                QString secid = exch_buyObj["secid"].toString();
                int lots_count = exch_buyObj["lots_count"].toInt();
                bool is_deposit = exch_buyObj["is_deposit"].toBool();
                QString purchase_datetime = exch_buyObj["purchase_datetime"].toString();
                QString account_id = exch_buyObj["account_id"].toString();
                double all_sum = exch_buyObj["all_sum"].toDouble();
                double average_price = exch_buyObj["average_price"].toDouble();

                emit show_purchase_history(secid,lots_count,is_deposit,purchase_datetime, account_id, all_sum, average_price);
            }
        }
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
        QMessageBox::information(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: Host not found.";
        QMessageBox::information(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: The connection was refused by the peer.";
        QMessageBox::information(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "Error: Socket operation timed out.";
        QMessageBox::information(nullptr, "Ошибка подключения", "Не удалось подключиться к серверу.");
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

QHash<QString, exchange_data>* Client::get_exchange_hash()
{
    return &exchange_hash;
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

void Client::GetBalanceWindow(const QJsonObject &data)
{
    // Добавляем тип данных "acc" в объект JSON
    QJsonObject messageData;
    messageData["type"] = "get_balance";
    messageData["data"] = data;

    // Преобразование объекта в строку JSON
    QJsonDocument jsonDoc(messageData);
    QString jsonData = jsonDoc.toJson();

    this->sendMessage(jsonData);
}

QHash<QString, marketdata_info>* Client::get_market_hash()
{
    return &marketdata_hash;
}

QHash<QString, purchase>* Client::get_purhase_hash()
{
    return &purchase_hash;
}


