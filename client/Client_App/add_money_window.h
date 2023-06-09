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
    explicit add_money_window(Client* add_client, QWidget *parent = nullptr);
    ~add_money_window();
    void onbackbutton();
    void onconfirmbutton();

private:
    Ui::add_money_window *ui;
    Client* add_client;
};

#endif // ADD_MONEY_WINDOW_H
