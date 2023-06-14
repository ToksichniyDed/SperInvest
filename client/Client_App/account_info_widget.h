#ifndef ACCOUNT_INFO_WIDGET_H
#define ACCOUNT_INFO_WIDGET_H

#include <QToolBox>
#include <QJsonObject>
#include <QTabBar>

#include "ui_account_info_widget.h"
#include "add_money_window.h"
#include "get_money_window.h"
#include "client.h"
#include "account_info.h"
#include "exch.h"
#include "exch_tab.h"

namespace Ui {
class account_info_widget;
}

class account_info_widget : public QToolBox
{
    Q_OBJECT

public:
    explicit account_info_widget(Client* a_client ,QString acc_id = "", QWidget *parent = nullptr,account_info* acc_info = nullptr);
    ~account_info_widget();

private:
    account_info* acc_info;
    Client* a_client;
    QString acc_id;
    Ui::account_info_widget *ui;
    void Show_Acc_Info();
    void Show_Exch_Info();
    void on_Add_Money_Clicked();
    void on_Get_Money_Clicked();
};



#endif // ACCOUNT_INFO_WIDGET_H
