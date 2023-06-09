#include "add_money_window.h"

add_money_window::add_money_window(Client* add_client, QWidget *parent) :
    QDialog(parent),ui(new Ui::add_money_window), add_client(add_client)
{
    ui->setupUi(this);
    connect(ui->back_button,&QPushButton::clicked,this,&add_money_window::onbackbutton);
    connect(ui->confirm_add,&QPushButton::clicked,this,&add_money_window::onconfirmbutton);
}

add_money_window::~add_money_window()
{
    delete ui;
}

void add_money_window::onbackbutton()
{
    this->close();
}

void add_money_window::onconfirmbutton()
{
    QString add_balance = findChild<QLineEdit*>("money_line")->text();

    if(add_balance.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполнитe поле.");
    }
    else
    {
        // Создание объекта с данными
        QJsonObject dataObject;
        //dataObject["account_id"] = account_ID;
        //dataObject["add_balance"] = add_balance;

        //а_client->AddBalanceWindow(dataObject);
    }
}
