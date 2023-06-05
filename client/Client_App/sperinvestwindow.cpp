#include "sperinvestwindow.h"

SperInvestWindow::SperInvestWindow(Client* client, QWidget *parent) : QTabWidget(parent), s_client(client)
{
    ui = new Ui::SperInvestWindow;
    ui->setupUi(this);

     enter_window *enterwindow = new enter_window(s_client, this);

    //Открываем главное окно
    show();

    // Открываем модальное диалоговое окно
    enterwindow->exec();
}

SperInvestWindow::~SperInvestWindow()
{
    delete ui;
}


