#include "create_account.h"

create_account::create_account(Client* client, QWidget *parent) : c_client(client), QDialog(parent), ui(new Ui::create_account)
{
    ui->setupUi(this);


}

create_account::~create_account()
{
    delete ui;
}
