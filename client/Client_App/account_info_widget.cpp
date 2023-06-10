#include "account_info_widget.h"

account_info_widget::account_info_widget(Client* a_client, QWidget *parent, account_info* acc_info) : QToolBox(parent),
    ui(new Ui::account_info_widget), acc_info(acc_info), a_client(a_client)
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
    QString account_balance = (*acc_info).get_account_balance();
    QString formatted_balance = QString::number(account_balance.toDouble(), 'f', 2);

    ui->money->setText(formatted_balance);
}

void account_info_widget::on_Add_Money_Clicked()
{
    double account_balance = ui->money->text().toDouble();
    add_money_window add_window(a_client,account_balance);
    add_window.setModal(true);
    add_window.show();
    add_window.exec();
}
