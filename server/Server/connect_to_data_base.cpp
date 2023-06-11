#include "connect_to_data_base.h"

DatabaseConnector::DatabaseConnector(QCoreApplication *app, QObject *parent)
    : QObject(parent), m_app(app)
{
}

void DatabaseConnector::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("SperInvest");
    db.setUserName("postgres");
    db.setPassword("password");

    if (db.open()) {
        QSqlQuery query;
        query.exec("SELECT version()");
        if (query.next()) {
            QString version = query.value(0).toString();
            qDebug() << "PostgreSQL version:" << version;
        }
    } else {
        qDebug() << "Failed to connect to the database.";
    }
}

void DatabaseConnector::disconnectFromDatabase()
{
    db.close();
    qDebug() << "Successful disconnection from the database.";
}

