#ifndef REGISTRATION_WINDOW_H
#define REGISTRATION_WINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

#include "ui_registration_window.h"
#include "client.h"

namespace Ui {
class registration_window;
}

class registration_window : public QDialog
{
    Q_OBJECT

public:
    explicit registration_window(Client *client, QWidget *parent = nullptr);
    void onRback_ButtonClicked();//Кнопка подтверждения регистрации
    void onBack_ButtonClicked();// Кнопка 'назад'
    void rec_from_server(QString message);//Ответ от сервера (подтверждение/отказ регистрации)
    ~registration_window();

private:
    Ui::registration_window *ui;
    Client* r_client;

};

#endif // REGISTRATION_WINDOW_H
