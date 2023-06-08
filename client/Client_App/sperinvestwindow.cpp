#include "sperinvestwindow.h"

SperInvestWindow::SperInvestWindow(Client* client, QWidget *parent) : QTabWidget(parent), s_client(client)
{
    ui = new Ui::SperInvestWindow;

    ui->setupUi(this);

    enter_window *enterwindow = new enter_window(s_client, this);

    //Подключаем сигналы и слоты
    connect(ui->push_account,&QPushButton::clicked,this,&SperInvestWindow::onCreate_Account_ButtonClicked);//Кнопка создания счета
    connect(s_client,&Client::send_to_Show_Accounts, this,&SperInvestWindow::Show_Accounts);//Поле вывода счетов
    connect(s_client,&Client::clear_accounts_window,this,&SperInvestWindow::clear_show_accounts_window);//Удаление кнопок счетов

    tabBar()->setExpanding(true);
    //    int tabWidth = width() / count(); // Равномерно распределить ширину на количество вкладок
    //    int tabHeight = tabBar()->height(); // Задать высоту вкладок
    //    for (int i = 0; i < count(); i++) {
    //        setTabFixedSize(i, tabWidth, tabHeight);
    //    }

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

void SperInvestWindow::Show_Accounts(QJsonObject& accountObject)
{
    QString accountName = accountObject["account_name"].toString();

    // Создание кнопки
    QPushButton* button = new QPushButton(accountName);

    // Получение существующего макета
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

    // Добавление кнопки в макет
    layout->addWidget(button);

    // Соединяем сигнал кнопки со слотом для обработки нажатия
    connect(button, &QPushButton::clicked, this, [this, accountObject]() {
        // Обработка нажатия кнопки для данного счета
        // Можно вызвать дополнительные действия или открыть новое окно
        // Используйте данные счета из объекта JSON по вашему усмотрению
    });
}



