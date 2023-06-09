#include "sperinvestwindow.h"

SperInvestWindow::SperInvestWindow(Client* client,QHash<QString, account_info>* acc_info_hash, QWidget *parent) : QTabWidget(parent), s_client(client), acc_info_hash(acc_info_hash)
{
    ui = new Ui::SperInvestWindow;

    ui->setupUi(this);

    enter_window *enterwindow = new enter_window(s_client, this);

    //Подключаем сигналы и слоты
    connect(ui->push_account,&QPushButton::clicked,this,&SperInvestWindow::onCreate_Account_ButtonClicked);//Кнопка создания счета
    connect(s_client,&Client::send_to_Show_Accounts, this,&SperInvestWindow::Show_Accounts);//Поле вывода счетов
    connect(s_client,&Client::clear_accounts_window,this,&SperInvestWindow::clear_show_accounts_window);//Удаление кнопок счетов
    connect(s_client,&Client::send_acc_info,this, &SperInvestWindow::set_account_info_hash);// Передаем счета клиента

    // Создание макета
    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Установка созданного макета в scrollAreaWidgetContents
    ui->scrollAreaWidgetContents->setLayout(layout);

    //Открываем главное окно
    show();

    // Открываем модальное диалоговое окно
    enterwindow->exec();
}

SperInvestWindow::~SperInvestWindow()
{
    delete ui;
}

void SperInvestWindow::onCreate_Account_ButtonClicked()
{
    create_account* create_window = new create_account(s_client, this);  
    create_window->exec();
}

void SperInvestWindow::clear_show_accounts_window()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        layout->removeWidget(widget);
        delete widget;
        delete item;
    }
}

//void SperInvestWindow::Show_Accounts(QJsonObject& accountObject)
//{
//    // Вывод общего баланса
//    double sum_balance = ui->accounts_balance->text().toDouble();
//    sum_balance += accountObject["account_balance"].toString().toDouble();
//    QString formatted_balance = QString::number(sum_balance, 'f', 2);
//    ui->accounts_balance->setText(formatted_balance);

//    QString accountName = accountObject["account_name"].toString();

//    // Создание кнопки
//    QPushButton* button = new QPushButton(accountName);

//    // Получение существующего макета
//    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

//    // Добавление кнопки в макет
//    layout->addWidget(button);

//    // Создание вкладки
//    QTabWidget* tabWidget = findChild<QTabWidget*>("acc_info_tab"); // Получение указателя на QTabWidget из главного окна
//    account_info_widget* acc_info_widget = new account_info_widget(s_client, tabWidget, &accountObject);
//    tabWidget->addTab(acc_info_widget, accountName);
//    tabWidget->setStyleSheet("QTabBar::tab { height: 0px; width: 0px; }");
//    tabWidget->setCurrentIndex(tabWidget->count()-1);
//    ui->current_account->setText(accountName);

//    connect(button, &QPushButton::clicked, this, [=]() {
//        activateTabByText(tabWidget, accountName);
//    });
//}

void SperInvestWindow::Show_Accounts(QString* acc_id)
{
    // Вывод общего баланса
    double sum_balance = ui->accounts_balance->text().toDouble();

    account_info acc_info = (*acc_info_hash).value(*acc_id);

    sum_balance += acc_info.get_account_balance().toDouble();
    QString formatted_balance = QString::number(sum_balance, 'f', 2);
    ui->accounts_balance->setText(formatted_balance);

    QString accountName = acc_info.get_account_name();

    // Создание кнопки
    QPushButton* button = new QPushButton(accountName);

    // Получение существующего макета
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

    // Добавление кнопки в макет
    layout->addWidget(button);

    // Создание вкладки
    QTabWidget* tabWidget = findChild<QTabWidget*>("acc_info_tab"); // Получение указателя на QTabWidget из главного окна
    account_info_widget* acc_info_widget = new account_info_widget(s_client, tabWidget, acc_info_hash);
    tabWidget->addTab(acc_info_widget, accountName);
    tabWidget->setStyleSheet("QTabBar::tab { height: 0px; width: 0px; }");
    tabWidget->setCurrentIndex(tabWidget->count()-1);
    ui->current_account->setText(accountName);

    connect(button, &QPushButton::clicked, this, [=]() {
        activateTabByText(tabWidget, accountName);
    });
}

void SperInvestWindow::activateTabByText(QTabWidget* tabWidget, const QString& tabText)
{
    ui->current_account->setText(tabText);
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == tabText) {
            tabWidget->setCurrentIndex(i);
            break;
        }
    }
}
void SperInvestWindow::set_account_info_hash(QHash<QString, account_info>* acc_info_hash)
{
    this->acc_info_hash = acc_info_hash;
}
