#include "buy_exchange.h"

buy_exchange::buy_exchange(Client* buy_client, QString secid, float result_sum, float price,int lotssize, QWidget *parent) :
    QDialog(parent), buy_client(buy_client), secid(secid), result_sum(result_sum),price(price),lotssize(lotssize),
    ui(new Ui::buy_exchange)
{
    ui->setupUi(this);

    QHash<QString, account_info>* accountHash = buy_client->get_acc_hash();

    for (const QString& accountKey : accountHash->keys()) {
        account_info accountInfo = accountHash->value(accountKey);
        QString accountinfo = QString("%1 (%2 %3)").arg(accountInfo.get_account_name()).arg(accountInfo.get_account_balance().toDouble()).arg(accountInfo.get_currency());

        ui->available_accs->addItem(accountinfo);
    }

    connect(ui->count_lots, &QLineEdit::textChanged, this, &buy_exchange::onFirstLineEditTextChanged);
    connect(ui->buy_but, &QPushButton::clicked, this, &buy_exchange::onBuyButton);
    connect(this, &buy_exchange::send_buy_exchange_information, buy_client,&Client::sendMessage);
    connect(buy_client, &Client::receivePurchaseExchangeSuccess, this, &buy_exchange::receivePurchaseExchange);
}

buy_exchange::~buy_exchange()
{
    disconnect(ui->count_lots, &QLineEdit::textChanged, this, &buy_exchange::onFirstLineEditTextChanged);
    disconnect(ui->buy_but, &QPushButton::clicked, this, &buy_exchange::onBuyButton);
    disconnect(this, &buy_exchange::send_buy_exchange_information, buy_client,&Client::sendMessage);
    disconnect(buy_client, &Client::receivePurchaseExchangeSuccess, this, &buy_exchange::receivePurchaseExchange);

    delete ui;
}

void buy_exchange::onFirstLineEditTextChanged(const QString& text)
{
    // Проверка на числовой тип данных
    bool isNumeric = false;
    QString lotsize = "";
    QString last = "";
    text.toDouble(&isNumeric);

    if (!isNumeric && !text.isEmpty()) {
        QMessageBox::critical(this, "Ошибка","Вводите только числовое значение!");
        ui->sum_lots->clear();
        ui->count_lots->clear();
    }
    else{
        QHash<QString, exchange_data>* exchangeHash = buy_client->get_exchange_hash();
        QHash<QString, marketdata_info>* marketdataHash = buy_client->get_market_hash();

        // Поиск значения в exchange_hash
        if (exchangeHash->contains(this->secid)) {
            exchange_data exchangeData = exchangeHash->value(secid);
            lotsize = exchangeData.getLOTSIZE();
            lotssize = lotsize.toInt();
        }

        // Поиск значения в marketdata_hash
        if (marketdataHash->contains(this->secid)) {
            marketdata_info marketData = marketdataHash->value(secid);
            last = marketData.getLAST();
        }

        float result1 = lotsize.toDouble()*last.toDouble()*text.toDouble();
        QString result = QString::number(result1);
        ui->sum_lots->setText(result);
        this->result_sum = result1;
        this->price = last.toFloat();
    }
}

void buy_exchange::onBuyButton()
{
    if(ui->count_lots->text().isEmpty())
    {
        QMessageBox::critical(this,"Ошибка","Заполните все поля!");
    }
    else{
        QHash<QString, account_info>* accountHash = buy_client->get_acc_hash();
        QString acc_id = "";

        for (const QString& accountKey : accountHash->keys()) {
            account_info accountInfo = accountHash->value(accountKey);
            QString comboBoxText = ui->available_accs->currentText();
            QString acc_name = comboBoxText.section('(', 0, 0, QString::SectionSkipEmpty).trimmed();

            if(accountInfo.get_account_name() == acc_name)
            {
                acc_id = accountInfo.get_account_id();
                if (this->result_sum > accountInfo.get_account_balance().toFloat())
                    QMessageBox::critical(this, "Ошибка","На данном счете недостаточно средств! Используйте другой или пополните счет!");
                else
                {
                    QString result_sum_q = QString::number(result_sum);

                    QJsonObject messageData;
                    messageData["type"] = "buy_exchange";
                    messageData["acc_id"] = acc_id;
                    messageData["secid"] = this->secid;
                    messageData["result_sum"]=result_sum_q;
                    int temp = ui->count_lots->text().toInt();
                    messageData["count_of_lots"] = QString::number(temp);
                    messageData["price"]= QString::number(this->price);
                    messageData["lotssize"]= QString::number(this->lotssize);

                    qDebug()<<"LOT: "<<QString::number(this->lotssize);

                    // Преобразование объекта в строку JSON
                    QJsonDocument jsonDoc(messageData);
                    QString jsonData = jsonDoc.toJson();
                    emit send_buy_exchange_information(jsonData);
                    break;
                }
            }
        }
    }
}

void buy_exchange::receivePurchaseExchange(QString message)
{
    if(message == "Успешная покупка!")
        {
        QMessageBox::information(this,"Поздравляю!", message);
        QJsonObject rec;
        rec["type"]= "update_purch";
        rec["data"] = "";
        QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
        QString update = QString::fromUtf8(byte_rec_log_data);

         qDebug()<<update;

         buy_client->sendMessage(update);
    }
    else if(message == "Успешная продажа!")
    {
        QMessageBox::information(this,"Поздравляю!", message);
        QJsonObject rec;
        rec["type"]= "update_purch";
        rec["data"] = "";
        QByteArray byte_rec_log_data = QJsonDocument(rec).toJson();
        QString update = QString::fromUtf8(byte_rec_log_data);

        qDebug()<<update;

        buy_client->sendMessage(update);
    }
    else if(message == "Ошибка при покупке!")
    {
         QMessageBox::information(this,"Ошибка", message);
    }
    else
    {
        QMessageBox::information(this,"Ошибка", message);
    }
    disconnect(buy_client, &Client::receivePurchaseExchangeSuccess, this, &buy_exchange::receivePurchaseExchange);
    this->close();
}

















