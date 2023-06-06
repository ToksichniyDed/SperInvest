#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <QDialog>

#include "ui_create_account_window.h"
#include "client.h"

namespace Ui {
class create_account;
}

class create_account : public QDialog
{
    Q_OBJECT

public:
    explicit create_account(Client* client,QWidget *parent = nullptr);
    ~create_account();
    void check_create_acc_data();
    void close_window(QString message);//Успешное создание счета

private:
    Ui::create_account *ui;
    Client* c_client;


};

#endif // CREATE_ACCOUNT_H
