#ifndef SPERINVESTWINDOW_H
#define SPERINVESTWINDOW_H

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHash>
#include <QTabBar>

#include "ui_sperinvestwindow.h"
#include "client.h"
#include "enter_window.h"
#include "create_account.h"
#include "account_info_widget.h"
#include "account_info.h"
#include "exchange_data.h"
#include "exchange_widget.h"

class SperInvestWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit SperInvestWindow(Client* client, QHash<QString, account_info>* acc_info_hash = nullptr,QHash<QString, exchange_data>* exchange_info_hash = nullptr, QWidget *parent = nullptr);
    ~SperInvestWindow();
    void onCreate_Account_ButtonClicked();//Кнопка создания нового счета
    void Show_Accounts(QString *acc_id);//Окно вывода счетов
    void clear_show_accounts_window();//Очистка окна счетов
    void clear_history_purchase_layout();
    void clear_money_history_layout();
    void Show_Account_Info();//Информация о счете
    void Show_Money_History(QString amount = "", QString transferDate = "",bool isDeposit = true, QString accountId = "");//Историй денежных операций
    void Show_Purchase_History(QString secid,int lots_count, bool is_deposit,QString purchase_datetime,QString  account_id,double all_sum,double average_price);//История сделок
    void activateTabByText(QTabWidget* tabWidget, const QString& tabText,const QString& sum);
    void set_account_info_hash(QHash<QString, account_info>* acc_info_hash);
    void set_exchange_info_hash(QHash<QString, exchange_data>* exchange_info);
    void set_marketdata_info(QHash<QString, marketdata_info>* marketdata_hash);
    void Show_Exchange_Issuer(QString exchange_id);
    void on_confirm_exchange_button_clicked();
    void closeTab(int index);

private:
    Ui::SperInvestWindow *ui;
    Client* s_client;
    QVBoxLayout* vLayout;
    QVBoxLayout* Layout;
    QHash<QString, account_info>* acc_info_hash;
    QHash<QString, exchange_data>* exchange_info_hash;
    QHash<QString, marketdata_info>* marketdata_info_hash;
};

#endif // SPERINVESTWINDOW_H
