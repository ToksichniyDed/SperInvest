#ifndef SPERINVESTWINDOW_H
#define SPERINVESTWINDOW_H

#include <QTabWidget>

#include "ui_sperinvestwindow.h"
#include "client.h"
#include "enter_window.h"

class SperInvestWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit SperInvestWindow(Client* client, QWidget *parent = nullptr);
    ~SperInvestWindow();

private:
    Ui::SperInvestWindow *ui;
    Client* s_client;

};

#endif // SPERINVESTWINDOW_H
