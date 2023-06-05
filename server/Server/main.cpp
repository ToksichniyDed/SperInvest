#include <QCoreApplication>
#include <iostream>
#include "server.h"
#include "connect_to_data_base.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseConnector connector(&a);
    connector.connectToDatabase();
    Server main_server;
    main_server.start();
    return a.exec();
    connector.disconnectFromDatabase();
}

