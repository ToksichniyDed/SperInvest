#include "sperinvestwindow.h"
#include"client.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    QHash <QString, account_info>* acc_hash = client.get_acc_hash();
    SperInvestWindow main_window (&client, acc_hash);
    return a.exec();
}
