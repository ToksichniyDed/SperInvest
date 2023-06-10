#include "enter_window.h"

enter_window::enter_window(Client *client, QWidget *parent) : QDialog(parent), ui(new Ui::enter_window), e_client(client)
{
    ui->setupUi(this);

    //Соединяем сигналы с кнопками
    connect(ui->reg_button, &QPushButton::clicked, this, &enter_window::onReg_ButtonClicked);
    connect(ui->log_button, &QPushButton::clicked, this, &enter_window::onLog_ButtonClicked);

    connect(e_client, &Client::close_enter_window, this, &enter_window::close_enter_window);
    connect(e_client, &Client::access_denied_enter_window, this, &enter_window::denied_access);
}

enter_window::~enter_window()
{
    delete ui;
}

void enter_window::onReg_ButtonClicked()
{
    registration_window reg_window(e_client);

    reg_window.exec();
}

void enter_window::onLog_ButtonClicked()
{
    QLineEdit* EmailField = findChild<QLineEdit*>("login");
    QLineEdit* PasswordField = findChild<QLineEdit*>("password");

    QString Email = EmailField->text();
    QString Password = PasswordField->text();

    if(Email.isEmpty() || Password.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполните все поля.");
    }
    else
    {
        e_client->connectToServer("127.0.0.1", 1234);
        // Создание объекта с данными
        QJsonObject dataObject;
        dataObject["Email"] = Email;
        dataObject["Password"] = Password;

        e_client-> enterWindowSubmit(dataObject);
    }


}

void enter_window::denied_access()
{
    QMessageBox::critical(this, "Ошибка", "Неправильный логин или пароль.");
}

void enter_window::close_enter_window()
{
    this->accept();
}
