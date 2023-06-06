#ifndef SPERINVESTWINDOW_H
#define SPERINVESTWINDOW_H

#include <QTabWidget>

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

private:
    Ui::SperInvestWindow *ui;
    Client* s_client;
    QString* em_user_id;

};

#endif // SPERINVESTWINDOW_H
