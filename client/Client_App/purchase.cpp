#include "purchase.h"

purchase::purchase(QString PURCHASE_ID, QString SECID, QString LOTS, QString AVERAGE_PRICE, QString PURCHASE_DATETIME, QString ACCOUNT_ID)
    : PURCHASE_ID(PURCHASE_ID),SECID(SECID), LOTS(LOTS), AVERAGE_PRICE(AVERAGE_PRICE), PURCHASE_DATETIME(PURCHASE_DATETIME), ACCOUNT_ID(ACCOUNT_ID)
{
    qDebug()<<"Create: "<<SECID<<" from "<< ACCOUNT_ID;
}

QString purchase::getPURCHASE_ID()
{
    return PURCHASE_ID;
}

void purchase::setPURCHASE_ID( QString &value)
{
    SECID = PURCHASE_ID;
}

QString purchase::getSECID()
{
    return SECID;
}

void purchase::setSECID( QString &value)
{
    SECID = value;
}

QString purchase::getLOTS()
{
    return LOTS;
}

void purchase::setLOTS( QString &value)
{
    LOTS = value;
}

QString purchase::getAVERAGE_PRICE()
{
    return AVERAGE_PRICE;
}

void purchase::setAVERAGE_PRICE( QString &value)
{
    AVERAGE_PRICE = value;
}

QString purchase::getPURCHASE_DATETIME()
{
    return PURCHASE_DATETIME;
}

void purchase::setPURCHASE_DATETIME( QString &value)
{
    PURCHASE_DATETIME = value;
}

QString purchase::getACCOUNT_ID()
{
    return ACCOUNT_ID;
}

void purchase::setACCOUNT_ID( QString &value)
{
    ACCOUNT_ID = value;
}
