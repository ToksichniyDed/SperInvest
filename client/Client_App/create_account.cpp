#include "create_account.h"

create_account::create_account(Client* client, QWidget *parent) : c_client(client), QDialog(parent), ui(new Ui::create_account)
{
    ui->setupUi(this);

    //Подключаем сигналы и слоты
    connect(ui->create_account_button,&QPushButton::clicked,this,&create_account::check_create_acc_data);
    connect(c_client,&Client::close_create_acc_window,this,&create_account::close_window);
}

create_account::~create_account()
{
    delete ui;
}

void create_account::check_create_acc_data()
{
    // Получение доступа к элементам формы
    QLineEdit* Acc_nameField = findChild<QLineEdit*>("account_name");
    QLineEdit* CurrencyField = findChild<QLineEdit*>("account_currency");
    QLineEdit* StartBalanceField = findChild<QLineEdit*>("start_balance");
    QLineEdit* TariffPlanField = findChild<QLineEdit*>("tariff_plan");

    // Получение значений полей
    QString acc_name = Acc_nameField->text();
    QString currency = CurrencyField->text();
    QString startbalance = StartBalanceField->text();
    QString tariffplan = TariffPlanField->text();

    if(acc_name.isEmpty() ||currency.isEmpty() || startbalance.isEmpty() || tariffplan.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполните все поля.");
    }
    else
    {
        // Создание объекта с данными
        QJsonObject dataObject;
        dataObject["acc_name"] = acc_name;
        dataObject["currency"] = currency;
        dataObject["startbalance"] = startbalance;
        dataObject["tariffplan"] = tariffplan;

        c_client->Create_Acc_WindowSubmit(dataObject);
    }
}

void create_account::close_window(QString message)
{
    QMessageBox::critical(this,"Поздравляю", message);
    this->accept();
}
