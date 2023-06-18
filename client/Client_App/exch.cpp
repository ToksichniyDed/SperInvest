#include "exch.h"

exch::exch(Client* exc_client,QString acc_id,purchase* purch, QWidget *parent) :
    QWidget(parent),exc_client(exc_client), acc_id(acc_id), purch(purch),
    ui(new Ui::exch)
{
    ui->setupUi(this);

    this->Show_Info();

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

exch::~exch()
{
    delete ui;
}

void exch::Show_Info()
{
    ui->SECID->setText(purch->getSECID());
    ui->LOTS->setText(purch->getLOTS());
    ui->AVERAGE_PRICE->setText(purch->getAVERAGE_PRICE());
    double based_price = purch->getLOTS().toDouble() * purch->getAVERAGE_PRICE().toDouble()*purch->getLOTSIZE().toDouble();
    ui->DIFFERENCE->setText(QString::number(based_price));
}
