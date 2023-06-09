#ifndef ACCOUNT_INFO_WIDGET_H
#define ACCOUNT_INFO_WIDGET_H

#include <QToolBox>
#include <QJsonObject>
#include <QHash>

#include "ui_account_info_widget.h"
#include "add_money_window.h"
#include "client.h"
#include "account_info.h"

namespace Ui {
class account_info_widget;
}

class account_info_widget : public QToolBox
{
    Q_OBJECT

public:
    explicit account_info_widget(Client* a_client , QWidget *parent = nullptr,QHash<QString,account_info>* acc_info = nullptr);
    ~account_info_widget();

private:
    QHash<QString,account_info>* acc_info;
    Client* a_client;
    Ui::account_info_widget *ui;
    void Show_Acc_Info();
    void on_Add_Money_Clicked();
};



#endif // ACCOUNT_INFO_WIDGET_H
