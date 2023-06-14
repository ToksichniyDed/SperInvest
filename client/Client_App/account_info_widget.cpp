#include "account_info_widget.h"

account_info_widget::account_info_widget(Client* a_client,QString acc_id, QWidget *parent, account_info* acc_info) : QToolBox(parent),
    ui(new Ui::account_info_widget), acc_info(acc_info), a_client(a_client), acc_id(acc_id)
{
    ui->setupUi(this);
    exch_tab* tab = new exch_tab(ui->page_2);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0, 0, 0, 0);
    tab->setLayout(layout);
    connect(ui->add_money,&QPushButton::clicked,this,&account_info_widget::on_Add_Money_Clicked);
    connect(ui->get_money, &QPushButton::clicked, this, &account_info_widget::on_Get_Money_Clicked);
    this->Show_Acc_Info();
}

account_info_widget::~account_info_widget()
{
    disconnect(ui->add_money,&QPushButton::clicked,this,&account_info_widget::on_Add_Money_Clicked);
    disconnect(ui->get_money, &QPushButton::clicked, this, &account_info_widget::on_Get_Money_Clicked);
    delete ui;
}

void account_info_widget::Show_Acc_Info()
{
    QString account_balance = (*acc_info).get_account_balance();
    QString formatted_balance = QString::number(account_balance.toDouble(), 'f', 2);

    ui->money->setText(formatted_balance);

    this->Show_Exch_Info();
}

void account_info_widget::Show_Exch_Info()
{
    QTabWidget* tab = ui->page_2->findChild<exch_tab*>("exch_tab");
    QHash<QString, purchase> *purchaseHash = a_client->get_purhase_hash();

    foreach (purchase purch, purchaseHash->values())
    {
        QString target_acc_id = purch.getACCOUNT_ID();

        if (acc_id == target_acc_id)
        {
            // acc_id счета совпадает, выполняем необходимые действия
            QString purchaseName = purch.getSECID();
            exch* exch_info = new exch(a_client,acc_id,&purch,tab);
            tab->addTab(exch_info, purchaseName);
            tab->setStyleSheet("QTabBar::tab{ height: 20px; width: 60px; }");
            tab->setCurrentIndex(1);
            QTabBar* tabBar = tab->tabBar();
            QString tabStyle = QString("QTabBar::tab:first { width: %1px; height: %2px; }").arg(0).arg(0);
            tabBar->setStyleSheet(tabStyle);
        }
    }
}

void account_info_widget::on_Add_Money_Clicked()
{

    add_money_window add_window(a_client,this->acc_id);
    add_window.setModal(true);
    add_window.show();
    add_window.exec();
}

void account_info_widget::on_Get_Money_Clicked()
{

    get_money_window add_window(a_client,this->acc_id);
    add_window.setModal(true);
    add_window.show();
    add_window.exec();
}
