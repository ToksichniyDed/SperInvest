#include "exchange_widget.h"

exchange_widget::exchange_widget(QWidget *parent,Client* ex_client,exchange_data* exch_info, marketdata_info* marketdata_info1) : ex_client(ex_client),exch_info(exch_info),
    QWidget(parent),ui(new Ui::exchange_widget), marketdata_info1(marketdata_info1)
{
    ui->setupUi(this);
    this->setExchangeData();

    connect(ex_client, &Client::update_marketdata_field, this, &exchange_widget::setMarketdataData);
    connect(ui->buy_exchange,&QPushButton::clicked, this, &exchange_widget::onBuy_Exch_ButtonClicked);
}

exchange_widget::~exchange_widget()
{
    disconnect(ex_client, &Client::update_marketdata_field, this, &exchange_widget::setMarketdataData);
    delete ui;    
}

void exchange_widget::setExchangeData()
{
    //Заполняем поля информации об акции
    QString temp = (*exch_info).getSECID();
    ui->SECID->setText(temp);
    temp = (*exch_info).getSECNAME();
    ui->SECNAME->setText(temp);
    temp = (*exch_info).getSHORTNAME();
    ui->SHORTNAME->setText(temp);
    temp = (*exch_info).getBOARDID();
    ui->BOARDID->setText(temp);
    temp = (*exch_info).getBOARDNAME();
    ui->BOARDNAME->setText(temp);
    temp = (*exch_info).getCURRENCYID();
    ui->CURRENCYID->setText(temp);
    temp = (*exch_info).getISIN();
    ui->ISIN->setText(temp);
    temp=(*exch_info).getLOTSIZE();
    ui->LOTSIZE->setText(temp);
    temp = (*exch_info).getPREVPRICE();
    ui->PREVPRICE->setText(temp);

    this->setMarketdataData(marketdata_info1);
}

void exchange_widget::setMarketdataData(marketdata_info* marketdata_info2)
{
    QString secid = ui->SECID->text();
    if((*marketdata_info2).getSECID() == secid)
    {
        QString temp = (*marketdata_info2).getOPEN();
        ui->OPEN->setText(temp);
        temp = (*marketdata_info2).getHIGH();
        ui->HIGH->setText(temp);
        temp = (*marketdata_info2).getLOW();
        ui->LOW->setText(temp);
        temp = (*marketdata_info2).getLAST();
        ui->LAST->setText(temp);
        temp = (*marketdata_info2).getSYSTIME();
        ui->SYSTIME->setText(temp);

        QJsonObject dataObject;
        dataObject["type"] = "update_marketdata";
        dataObject["data"] = (*marketdata_info2).getSECID();
        QJsonDocument doc(dataObject);
        QString jsonString = QString::fromUtf8(doc.toJson());

        QTimer::singleShot(10000, this, [this, jsonString]() {
            ex_client->sendMessage(jsonString);
        });
    }
}

void exchange_widget::onBuy_Exch_ButtonClicked()
{
    QString secid = ui->SECID->text();
    buy_exchange* buy_exchange_window = new buy_exchange(ex_client,secid,0,0,0,this);
    buy_exchange_window->setWindowTitle(secid);
    buy_exchange_window->exec();
}
