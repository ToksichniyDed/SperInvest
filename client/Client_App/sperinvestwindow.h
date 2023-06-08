#ifndef SPERINVESTWINDOW_H
#define SPERINVESTWINDOW_H

#include <QTabWidget>
#include <QVBoxLayout>

#include "ui_sperinvestwindow.h"
#include "client.h"
#include "enter_window.h"
#include "create_account.h"

class SperInvestWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit SperInvestWindow(Client* client, QWidget *parent = nullptr);
    ~SperInvestWindow();
    void onCreate_Account_ButtonClicked();//Кнопка создания нового счета
    void Show_Accounts(QJsonObject &accountObject);//Окно вывода счетов
    void clear_show_accounts_window();//Очитска окна счетов

private:
    Ui::SperInvestWindow *ui;
    Client* s_client;
    QString* sm_user_id;
};

#endif // SPERINVESTWINDOW_H
