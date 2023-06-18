#include "sperinvestwindow.h"

SperInvestWindow::SperInvestWindow(Client* client,QHash<QString, account_info>* acc_info_hash, QHash<QString, exchange_data>* exchange_info_hash, QWidget *parent) : QTabWidget(parent),
    s_client(client), acc_info_hash(acc_info_hash), exchange_info_hash(exchange_info_hash)
{
    ui = new Ui::SperInvestWindow;

    ui->setupUi(this);

    enter_window *enterwindow = new enter_window(s_client, this);

    //Подключаем сигналы и слоты
    connect(ui->push_account,&QPushButton::clicked,this,&SperInvestWindow::onCreate_Account_ButtonClicked);//Кнопка создания счета
    connect(ui->confirm_button,&QPushButton::clicked,this,&SperInvestWindow::on_confirm_exchange_button_clicked);//Кнопка вывода информации о бумаге
    connect(s_client,&Client::send_to_Show_Accounts, this,&SperInvestWindow::Show_Accounts);//Поле вывода счетов

    connect(s_client, &Client::send_to_Show_Exchange, this, &SperInvestWindow::Show_Exchange_Issuer);//Поле вывода акций
    connect(s_client,&Client::clear_accounts_window,this,&SperInvestWindow::clear_show_accounts_window);//Удаление кнопок счетов
    connect(s_client, &Client::clear_history_money, this, &SperInvestWindow::clear_money_history_layout);
    connect(s_client, &Client::clear_purch_history, this, &SperInvestWindow::clear_history_purchase_layout);
    connect(s_client,&Client::send_acc_info,this, &SperInvestWindow::set_account_info_hash);// Передаем счета клиента
    connect(s_client,&Client::send_exchange_info,this,&SperInvestWindow::set_exchange_info_hash);//Передаем даннные с MOEX
    connect(s_client,&Client::send_marketdata_info,this,&SperInvestWindow::set_marketdata_info);
    connect(s_client,&Client::show_money_history, this, &SperInvestWindow::Show_Money_History);//История операций
    connect(s_client, &Client::show_purchase_history, this, &SperInvestWindow::Show_Purchase_History);//История сделок
    QTabWidget* tabWidget = findChild<QTabWidget*>("exchange_info");
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &SperInvestWindow::closeTab);// Кнопка закрытия вкладок о Акции

    // Создание макета
    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    Layout = new QVBoxLayout;
    Layout->setAlignment(Qt::AlignTop);
    Layout->setContentsMargins(0, 0, 0, 0);
    Layout->setSpacing(0);

    // Установка созданного макета в scrollAreaWidgetContents
    ui->scrollAreaWidgetContents->setLayout(layout);

    //Открываем главное окно
    show();

    // Устанавливаем размер вкладки в 0x0

    // Открываем модальное диалоговое окно
    enterwindow->setModal(true);
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
    QTabWidget* tabWidget = ui->acc_info_tab;
    int tabCount = tabWidget->count();

    // Удаление вкладок, начиная с индекса 1
    for (int i = tabCount - 1; i > 0; --i) {
        QWidget* tab = tabWidget->widget(i);
        tabWidget->removeTab(i);
        delete tab;
    }
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        layout->removeWidget(widget);
        delete widget;
        delete item;
    }
    ui->accounts_balance->clear();
    ui->current_balance->clear();
}

void SperInvestWindow::clear_history_purchase_layout()
{
    QLayoutItem* item;

    while ((item = Layout->takeAt(0)) != nullptr)
    {
        QWidget* widget = item->widget();
        if (widget)
        {
            Layout->removeWidget(widget);
            delete widget; // Удаление элемента
        }
        //delete item; // Удаление объекта QLayoutItem
    }
}

void SperInvestWindow::clear_money_history_layout()
{
    QLayoutItem* item;

    while ((item = vLayout->takeAt(0)) != nullptr)
    {
        QWidget* widget = item->widget();
        if (widget)
        {
            vLayout->removeWidget(widget);
            delete widget; // Удаление элемента
        }
        //delete item; // Удаление объекта QLayoutItem
    }
}

void SperInvestWindow::Show_Accounts(QString* acc_id)
{
    // Вывод общего баланса
    long double sum_balance_of_user = ui->accounts_balance->text().toDouble();

    long double sum_balance_of_acc = 0;

    account_info acc_info = (*acc_info_hash).value(*acc_id);

    QHash<QString, purchase> *purchaseHash = s_client->get_purhase_hash();

    foreach (purchase purch, purchaseHash->values())
    {
        QString acc_id_p = purch.getACCOUNT_ID();

        if (acc_id_p == acc_info.get_account_id())
        {
            sum_balance_of_acc += purch.getLOTS().toInt()*purch.getAVERAGE_PRICE().toDouble()*purch.getLOTSIZE().toDouble();
        }
    }

    sum_balance_of_acc += acc_info.get_account_balance().toDouble();

    sum_balance_of_user += acc_info.get_account_balance().toDouble();
    std::string stringValue = std::to_string(sum_balance_of_user);
    QString formatted_balance = QString::fromStdString(stringValue);
    ui->accounts_balance->setText(formatted_balance);
    stringValue = std::to_string(sum_balance_of_acc);
    QString formatted_acc_balance = QString::fromStdString(stringValue);
    ui->current_balance->setText(formatted_acc_balance);

    QString accountName = acc_info.get_account_name();
    QString accountId = acc_info.get_account_id();

    // Создание кнопки
    QPushButton* button = new QPushButton(accountName);

    // Получение существующего макета
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());

    // Добавление кнопки в макет
    layout->addWidget(button);

    // Создание вкладки
    QTabWidget* tabWidget = findChild<QTabWidget*>("acc_info_tab"); // Получение указателя на QTabWidget из главного окна
    account_info_widget* acc_info_widget = new account_info_widget(s_client,accountId,tabWidget, &acc_info);
    tabWidget->addTab(acc_info_widget, accountName);
    tabWidget->setStyleSheet("QTabBar::tab { height: 0px; width: 0px; }");
    tabWidget->setCurrentIndex(tabWidget->count()-1);
    ui->current_account->setText(accountName);

    connect(button, &QPushButton::clicked, this, [=]() {
        activateTabByText(tabWidget, accountName,formatted_acc_balance);
    });
}

void SperInvestWindow::Show_Money_History(QString amount, QString transferDate, bool isDeposit, QString accountId)
{
    QScrollArea* moneyHistoryScrollArea = findChild<QScrollArea*>("money_history");
    QWidget* scrollAreaContents = moneyHistoryScrollArea->findChild<QWidget*>("scrollAreaWidgetContents_3");

    QString acc_name = "";
    QString acc_currency = "";

    if (acc_info_hash->contains(accountId)) {

        account_info info = acc_info_hash->value(accountId);
        acc_name = info.get_account_name();
        acc_currency = info.get_currency();
    }
    QString formattedAmount = QString("%1").arg(amount.toDouble(), 0, 'f', 2); // Форматирование суммы

    QString transferType = isDeposit ? "Зачисление средств" : "Вывод средств"; // Тип операции
    QString transferType1 = isDeposit ? "на счет" : "со счета";
    QString transferText = QString("%1 %2 %3 %4: %5, %6")
                               .arg(transferType)
                               .arg(formattedAmount)
                               .arg(acc_currency)
                               .arg(transferType1)
                               .arg(acc_name)
                               .arg(transferDate);
    QLineEdit* lineEdit = new QLineEdit(scrollAreaContents);
    lineEdit->setText(transferText);
    lineEdit->setReadOnly(true);
    vLayout->addWidget(lineEdit);
    scrollAreaContents->setLayout(vLayout);
}

void SperInvestWindow::Show_Purchase_History(QString secid,int lots_count, bool is_deposit,QString purchase_datetime,QString  account_id,double all_sum,double average_price)
{
    QScrollArea* purchaseHistoryScrollArea = findChild<QScrollArea*>("purchase_history");
    QWidget* scrollAreaContents = purchaseHistoryScrollArea->findChild<QWidget*>("scrollAreaWidgetContents_2");

    QString transferType = is_deposit ? "Покупка" : "Продажа"; // Тип операции
        QString secid_name = "";
    QString secid_currency = "";
    QString lotsize = "";

    QString acc_name = "";
    QString acc_currency = "";

    if (acc_info_hash->contains(account_id)) {

        account_info info = acc_info_hash->value(account_id);
        acc_name = info.get_account_name();
        acc_currency = info.get_currency();
    }

    if (exchange_info_hash->contains(secid)) {

        exchange_data info = exchange_info_hash->value(secid);
        secid_name = info.getSHORTNAME();
        secid_currency = info.getCURRENCYID();
        lotsize = info.getLOTSIZE();
    }

    QString transferText = QString("%1 : %2 -> %3 %4 лотов (%5 акций в лоте) по средней цене %6 на сумму %7 %8 %9.")
                               .arg(acc_name)
                               .arg(secid)
                               .arg(transferType)
                               .arg(lots_count)
                               .arg(lotsize)
                               .arg(average_price)
                               .arg(all_sum)
                               .arg(acc_currency)
                               .arg(purchase_datetime);

    QLineEdit* lineEdit = new QLineEdit(scrollAreaContents);
    lineEdit->setText(transferText);
    lineEdit->setReadOnly(true);
    Layout->addWidget(lineEdit);
    scrollAreaContents->setLayout(Layout);
}

void SperInvestWindow::activateTabByText(QTabWidget* tabWidget, const QString& tabText,const QString& sum)
{
    ui->current_account->setText(tabText);
    ui->current_balance->setText(sum);

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

void SperInvestWindow::set_exchange_info_hash(QHash<QString, exchange_data>* exchange_info)
{
    this->exchange_info_hash = exchange_info;
}

void SperInvestWindow::set_marketdata_info(QHash<QString, marketdata_info> *marketdata_hash)
{
    this->marketdata_info_hash = marketdata_hash;
}

void SperInvestWindow::Show_Exchange_Issuer(QString exchange_id)
{
    exchange_data exch_info = (*exchange_info_hash).value(exchange_id);
    marketdata_info market_info = (*marketdata_info_hash).value(exchange_id);
    QString exchangeName = exch_info.getSECID();

    // Поиск существующей вкладки с таким именем
    QTabWidget* tabWidget = findChild<QTabWidget*>("exchange_info");
    int existingTabIndex = -1;
    for (int i = 0; i < tabWidget->count(); ++i) {
        if (tabWidget->tabText(i) == exchangeName) {
            existingTabIndex = i;
            break;
        }
    }

    if (existingTabIndex != -1) {
        // Устанавливаем текущую вкладку на существующую
        tabWidget->setCurrentIndex(existingTabIndex);
    } else {
        // Проверяем, существует ли вкладка с таким именем в ComboBox
        QString comboBoxText = ui->choose_issuer->currentText();
        QString comboBoxExchangeName = comboBoxText.left(comboBoxText.indexOf('('));
        if (comboBoxExchangeName == exchangeName) {
            // Создаем новую вкладку только если вкладка с таким именем не существует
            exchange_widget* exch_info_widget = new exchange_widget(tabWidget, s_client, &exch_info, &market_info);
            tabWidget->addTab(exch_info_widget, exchangeName);
            tabWidget->setCurrentIndex(tabWidget->count() - 1);

            // Скрываем иконку и устанавливаем размер вкладки First_tab в 0x0
            QTabBar* tabBar = tabWidget->tabBar();
            int firstTabIndex = 0; // Индекс вкладки "First_tab"

            tabBar->setTabIcon(firstTabIndex, QIcon());
            tabBar->setTabText(firstTabIndex, QString());
            tabBar->setStyleSheet(QString("QTabBar::tab:first { width: 0; height: 0; margin: 0; padding: 0; }"));
        }
    }
}

void SperInvestWindow::closeTab(int index)
{
    QTabWidget* tabWidget = findChild<QTabWidget*>("exchange_info");
    QWidget* tabContent = tabWidget->widget(index);
    // Выполните необходимые действия для закрытия вкладки, например, удалите вкладку или скройте ее содержимое.
    tabWidget->removeTab(index);
    delete tabContent;
}

void SperInvestWindow::on_confirm_exchange_button_clicked()
{
    QString exchangeName = ui->choose_issuer->currentText();
    exchangeName = exchangeName.left(exchangeName.indexOf('('));
    QJsonObject dataObject;
    dataObject["type"] = "get_exchange";
    dataObject["data"] = exchangeName;

    QJsonDocument jsonDoc(dataObject);
    QString jsonData = jsonDoc.toJson();

    s_client->sendMessage(jsonData);
}

