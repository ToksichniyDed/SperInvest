#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QCoreApplication>
#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QtSql/QSqlDriver>

class DatabaseConnector : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConnector(QCoreApplication *app, QObject *parent = nullptr);

public slots:
    void connectToDatabase();
    void disconnectFromDatabase();

private:
    QCoreApplication *m_app;
    QSqlDatabase db;
};

#endif // DATABASECONNECTOR_H
