#include "registration_window.h"

registration_window::registration_window(Client *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registration_window), r_client(client)
{
    ui->setupUi(this);

    QString phonePattern = "^\\d\\d{3}\\d{3}\\d{4}$";
    QRegularExpression phoneRegExp(phonePattern);
    QRegularExpressionValidator* phoneValidator = new QRegularExpressionValidator(phoneRegExp, this);
    QLineEdit* PhoneField = findChild<QLineEdit*>("Phone");

    PhoneField->setValidator(phoneValidator);


    // Связывание обработчика события с кнопками
    connect(ui->rback_button, &QPushButton::clicked, this, &registration_window::onRback_ButtonClicked);
    connect(ui->back_button, &QPushButton::clicked, this, &registration_window::onBack_ButtonClicked);
    connect(r_client,&Client::rec_reg_window,this,&registration_window::rec_from_server);
}

void registration_window::onRback_ButtonClicked()
{
    // Получение доступа к элементам формы
    QLineEdit* UsernameField = findChild<QLineEdit*>("Username");
    QLineEdit* EmailField = findChild<QLineEdit*>("Email");
    QLineEdit* PasswordField = findChild<QLineEdit*>("Password");
    QLineEdit* PhoneField = findChild<QLineEdit*>("Phone");

    // Получение значений полей
    QString Username = UsernameField->text();
    QString Email = EmailField->text();
    QString Password = PasswordField->text();
    QString Phone = PhoneField->text();

    if(Username.isEmpty() || Email.isEmpty() || Password.isEmpty() || Phone.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, заполните все поля.");
    }
    else
    {
        r_client->connectToServer("127.0.0.1", 1234);
        // Создание объекта с данными
        QJsonObject dataObject;
        dataObject["Username"] = Username;
        dataObject["Email"] = Email;
        dataObject["Password"] = Password;
        dataObject["Phone"] = Phone;

        r_client->registrationWindowSubmit(dataObject);
    }
}



void registration_window::onBack_ButtonClicked()
{
    this->reject();
}

void registration_window::rec_from_server(QString message)
{
    if(message == "Пользователь с таким логином уже существует.")
        QMessageBox::critical(this,"Ошибка", message);
    else
    {
        QMessageBox::information(this,"Поздравляю", message);
        this->accept();
    }
}

registration_window::~registration_window()
{
    delete ui;
}
