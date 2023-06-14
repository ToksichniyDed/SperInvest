#ifndef GET_MONEY_WINDOW_H
#define GET_MONEY_WINDOW_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>

#include "ui_add_money_window.h"
#include "client.h"
#include "ui_get_money_window.h"

namespace Ui {
class get_money_window;
}

class get_money_window : public QDialog
{
    Q_OBJECT

public:
    explicit get_money_window(Client* get_client,QString acc_id, QWidget *parent = nullptr);
    ~get_money_window();
    void rec_from_server(QString message);
    void onbackbutton();
    void onconfirmbutton();

private:
    Ui::get_money_window *ui;
    Client* get_client;
    QString acc_id;
};

#endif // GET_MONEY_WINDOW_H
