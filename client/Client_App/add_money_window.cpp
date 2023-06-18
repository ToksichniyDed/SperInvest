#include "add_money_window.h"

add_money_window::add_money_window(Client* add_client, QString acc_id, QWidget *parent) :
    QDialog(parent),ui(new Ui::add_money_window), add_client(add_client), acc_id(acc_id)
{
    ui->setupUi(this);
    connect(ui->back_button,&QPushButton::clicked,this,&add_money_window::onbackbutton);
    connect(ui->confirm_add,&QPushButton::clicked,this,&add_money_window::onconfirmbutton);
    connect(add_client, &Client::rec_add_money_window,this,&add_money_window::rec_from_server);
}

add_money_window::~add_money_window()
{
    disconnect(ui->back_button,&QPushButton::clicked,this,&add_money_window::onbackbutton);
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
        dataObject["account_id"] = this->acc_id;
        dataObject["add_balance"] = add_balance;

        add_client->AddBalanceWindow(dataObject);
    }
}

void add_money_window::rec_from_server(QString message)
{
    if(message == "Ошибка зачисления средств!")
        QMessageBox::information(this,"Ошибка", message);
    else
    {
        QMessageBox::information(this,"Поздравляю", message);
        disconnect(add_client, &Client::rec_add_money_window,this,&add_money_window::rec_from_server);
        this->close();
    }
}




