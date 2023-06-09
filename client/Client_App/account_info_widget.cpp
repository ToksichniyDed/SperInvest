#include "account_info_widget.h"

account_info_widget::account_info_widget(Client* a_client, QWidget *parent, QHash<QString,account_info>* acc_info) : QToolBox(parent),
    ui(new Ui::account_info_widget), acc_info(acc_info)
{
    ui->setupUi(this);
    connect(ui->add_money,&QPushButton::clicked,this,&account_info_widget::on_Add_Money_Clicked);
    this->Show_Acc_Info();
}

account_info_widget::~account_info_widget()
{
    delete ui;
}

void account_info_widget::Show_Acc_Info()
{
//    QString account_balance = this->acc_data->value("account_balance").toString();
//    QString formatted_balance = QString::number(account_balance.toDouble(), 'f', 2);

//    ui->money->setText(formatted_balance);
}

void account_info_widget::on_Add_Money_Clicked()
{
    add_money_window add_window(a_client);
    add_window.show();
    add_window.exec();
}
