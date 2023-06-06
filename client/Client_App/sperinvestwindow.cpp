#include "sperinvestwindow.h"

SperInvestWindow::SperInvestWindow(Client* client, QWidget *parent) : QTabWidget(parent), s_client(client)
{
    ui = new Ui::SperInvestWindow;

    ui->setupUi(this);

    enter_window *enterwindow = new enter_window(s_client, this);

    //Подключаем сигналы и слоты
    connect(ui->push_account,&QPushButton::clicked,this,&SperInvestWindow::onCreate_Account_ButtonClicked);

    //Открываем главное окно
    show();

    // Открываем модальное диалоговое окно
    enterwindow->exec();
}

SperInvestWindow::~SperInvestWindow()
{
    delete ui;
}

void SperInvestWindow::onCreate_Account_ButtonClicked()
{
    create_account *create_window = new create_account(s_client, this);

    create_window->exec();
}
