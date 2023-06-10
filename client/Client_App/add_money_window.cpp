#include "add_money_window.h"

add_money_window::add_money_window(Client* add_client, double account_balance, QWidget *parent) :
    QDialog(parent),ui(new Ui::add_money_window), add_client(add_client), account_balance(account_balance)
{
    ui->setupUi(this);
    connect(ui->back_button,&QPushButton::clicked,this,&add_money_window::onbackbutton);
    connect(ui->confirm_add,&QPushButton::clicked,this,&add_money_window::onconfirmbutton);
    connect(add_client, &Client::rec_add_money_window,this,&add_money_window::rec_from_server);
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

    QString for_bal = QString::number(account_balance);

    if(add_balance.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполнитe поле.");
    }
    else
    {
        // Создание объекта с данными
        QJsonObject dataObject;
        QString acc_id = findAccountIdByBalance(add_client->get_acc_hash(), for_bal);
        dataObject["account_id"] = acc_id;
        dataObject["add_balance"] = add_balance;

        add_client->AddBalanceWindow(dataObject);
    }
}

void add_money_window::rec_from_server(QString message)
{
    if(message == "Ошибка зачисления средств!")
        QMessageBox::critical(this,"Ошибка", message);
    else
    {
        QMessageBox::critical(this,"Поздравляю", message);
        this->accept();
    }
}

QString add_money_window::findAccountIdByBalance(QHash<QString, account_info>* accountHash, const QString& targetBalance)
{
    // Проходим по всем элементам хэша
    for (auto it = accountHash->begin(); it != accountHash->end(); ++it)
    {
        const QString& accountId = it.key();
        account_info& accountInfo = it.value();

        // Проверяем, совпадает ли баланс счета с заданным
        if (accountInfo.get_account_balance() == targetBalance)
        {
            return accountId; // Возвращаем айди счета
        }
    }

    return QString(); // Если счет с нужным балансом не найден
}



