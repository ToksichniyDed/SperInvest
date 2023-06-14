#include "get_money_window.h"

get_money_window::get_money_window(Client* get_client, QString acc_id, QWidget *parent) :
    QDialog(parent),get_client(get_client), acc_id(acc_id),
    ui(new Ui::get_money_window)
{
    ui->setupUi(this);
    connect(ui->back_button,&QPushButton::clicked,this,&get_money_window::onbackbutton);
    connect(ui->confirm_add,&QPushButton::clicked,this,&get_money_window::onconfirmbutton);
    connect(get_client, &Client::rec_add_money_window,this,&get_money_window::rec_from_server);
}

get_money_window::~get_money_window()
{
    disconnect(ui->back_button,&QPushButton::clicked,this,&get_money_window::onbackbutton);
    disconnect(ui->confirm_add,&QPushButton::clicked,this,&get_money_window::onconfirmbutton);
    disconnect(get_client, &Client::rec_add_money_window,this,&get_money_window::rec_from_server);
    delete ui;
}

void get_money_window::onbackbutton()
{
    this->close();
}

void get_money_window::onconfirmbutton()
{
    QString add_balance = findChild<QLineEdit*>("money_line")->text();

    if(add_balance.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполнитe поле.");
    }
    else
    {
//        if(add_balance.toDouble())
//        { QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполнитe поле.");
//            break;}
        // Создание объекта с данными
        QJsonObject dataObject;
        dataObject["account_id"] = this->acc_id;
        dataObject["add_balance"] = add_balance;

        get_client->GetBalanceWindow(dataObject);
    }
}

void get_money_window::rec_from_server(QString message)
{
    if(message == "Ошибка снятия средств!")
        QMessageBox::information(this,"Ошибка", message);
    else
    {
        QMessageBox::information(this,"Поздравляю", message);
        this->accept();
    }
}
