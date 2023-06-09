#ifndef SPERINVESTWINDOW_H
#define SPERINVESTWINDOW_H

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHash>

#include "ui_sperinvestwindow.h"
#include "client.h"
#include "enter_window.h"
#include "create_account.h"
#include "account_info_widget.h"
#include "account_info.h"

class SperInvestWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit SperInvestWindow(Client* client, QHash<QString, account_info>* acc_info_hash = nullptr, QWidget *parent = nullptr);
    ~SperInvestWindow();
    void onCreate_Account_ButtonClicked();//Кнопка создания нового счета
//    void Show_Accounts(QJsonObject &accountObject);//Окно вывода счетов
    void Show_Accounts(QString *acc_id);//Окно вывода счетов
    void clear_show_accounts_window();//Очистка окна счетов
    void Show_Account_Info();//Информация о счете
    void activateTabByText(QTabWidget* tabWidget, const QString& tabText);
    void set_account_info_hash(QHash<QString, account_info>* acc_info_hash);

private:
    Ui::SperInvestWindow *ui;
    Client* s_client;
    QHash<QString, account_info>* acc_info_hash;
};

#endif // SPERINVESTWINDOW_H
