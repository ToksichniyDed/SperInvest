#ifndef ENTER_WINDOW_H
#define ENTER_WINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

#include "ui_enter_window.h"
#include "registration_window.h"
#include "client.h"

class enter_window : public QDialog
{
    Q_OBJECT

public:
    explicit enter_window(Client *client, QWidget *parent = nullptr);
    ~enter_window();
    void onReg_ButtonClicked();
    void onLog_ButtonClicked();
    void denied_access();
    void close_enter_window();

private:
    Ui::enter_window *ui;
    Client* e_client;
};

#endif // ENTER_WINDOW_H
