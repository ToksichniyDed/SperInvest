#ifndef ADD_MONEY_WINDOW_H
#define ADD_MONEY_WINDOW_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>

#include "ui_add_money_window.h"
#include "client.h"

namespace Ui {
class add_money_window;
}

class add_money_window : public QDialog
{
    Q_OBJECT

public:
    explicit add_money_window(Client* add_client,double account_balance, QWidget *parent = nullptr);
    void rec_from_server(QString message);
    ~add_money_window();
    void onbackbutton();
    void onconfirmbutton();
    QString findAccountIdByBalance(QHash<QString, account_info>* accountHash, const QString& targetBalance);

private:
    Ui::add_money_window *ui;
    Client* add_client;
    double account_balance;
};

#endif // ADD_MONEY_WINDOW_H
