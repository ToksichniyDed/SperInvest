#include "exch.h"

exch::exch(Client* exc_client,QString acc_id,purchase* purch, QWidget *parent) :
    QWidget(parent),exc_client(exc_client), acc_id(acc_id), purch(purch),
    ui(new Ui::exch)
{
    ui->setupUi(this);

    this->Show_Info();
}

exch::~exch()
{
    delete ui;
}

void exch::Show_Info()
{
    double size = 1;
    ui->SECID->setText(purch->getSECID());
    ui->LOTS->setText(purch->getLOTS());
    ui->AVERAGE_PRICE->setText(purch->getAVERAGE_PRICE());
    QHash <QString, exchange_data>* exchange = exc_client->get_exchange_hash();
    if(exchange->contains(purch->getSECID()))
        size = exchange->value(purch->getSECID()).getLOTSIZE().toDouble();
    double based_price = purch->getLOTS().toDouble() * purch->getAVERAGE_PRICE().toDouble()*size;
    ui->DIFFERENCE->setText(QString::number(based_price));
}
