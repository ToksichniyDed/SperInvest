#include "sperinvestwindow.h"
#include"client.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    client.connectToServer("127.0.0.1", 1234);
    SperInvestWindow main_window (&client);
    return a.exec();
}
