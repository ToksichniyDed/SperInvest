#include "sperinvestwindow.h"
#include "client.h"
#include "exchange_data.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    QHash <QString, account_info>* acc_hash = client.get_acc_hash();
    QHash <QString, exchange_data>* exch_hash = client.get_exchange_hash();
    SperInvestWindow main_window (&client, acc_hash, exch_hash);
    return a.exec();
}
