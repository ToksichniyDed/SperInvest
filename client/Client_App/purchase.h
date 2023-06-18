#ifndef PURCHASE_H
#define PURCHASE_H

#include <QString>
#include <QDebug>

class purchase
{
public:
    purchase(QString PURCHASE_ID = "",
             QString SECID = "",
             QString LOTS = "",
             QString LOTSIZE = "",
             QString AVERAGE_PRICE = "",
             QString PURCHASE_DATETIME = "",
             QString ACCOUNT_ID = ""
);

    QString getPURCHASE_ID() ;
    void setPURCHASE_ID( QString &value);

    QString getSECID() ;
    void setSECID( QString &value);

    QString getLOTS() ;
    void setLOTS( QString &value);

    QString getLOTSIZE() ;
    void setLOTSIZE( QString &value);

    QString getAVERAGE_PRICE() ;
    void setAVERAGE_PRICE( QString &value);

    QString getPURCHASE_DATETIME() ;
    void setPURCHASE_DATETIME( QString &value);

    QString getACCOUNT_ID() ;
    void setACCOUNT_ID( QString &value);



private:
    QString PURCHASE_ID;
    QString SECID;
    QString LOTS;
    QString LOTSIZE;
    QString AVERAGE_PRICE;
    QString PURCHASE_DATETIME;
    QString ACCOUNT_ID;
};

#endif // PURCHASE_H
