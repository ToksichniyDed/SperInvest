#ifndef EXCHANGE_DATA_H
#define EXCHANGE_DATA_H

#include <QString>

class exchange_data
{
public:
    exchange_data(QString SECID = "",
                  QString BOARDID = "",
                  QString SHORTNAME = "",
                  QString PREVPRICE = "",
                  QString LOTSIZE = "",
                  QString FACEVALUE = "",
                  QString STATUS = "",
                  QString BOARDNAME = "",
                  QString DECIMALS = "",
                  QString SECNAME = "",
                  QString REMARKS = "",
                  QString MARKETCODE = "",
                  QString INSTRID = "",
                  QString SECTORID = "",
                  QString MINSTEP = "",
                  QString PREVWAPRICE = "",
                  QString FACEUNIT = "",
                  QString PREVDATE = "",
                  QString ISSUESIZE = "",
                  QString ISIN = "",
                  QString LATNAME = "",
                  QString REGNUMBER = "",
                  QString PREVLEGALCLOSEPRICE = "",
                  QString CURRENCYID = "",
                  QString SECTYPE = "",
                  QString LISTLEVEL = "",
                  QString SETTLEDATE = "");

    QString getSECID();
    QString getBOARDID();
    QString getSHORTNAME();
    QString getPREVPRICE();
    QString getLOTSIZE();
    QString getFACEVALUE();
    QString getSTATUS();
    QString getBOARDNAME();
    QString getDECIMALS();
    QString getSECNAME();
    QString getREMARKS();
    QString getMARKETCODE();
    QString getINSTRID();
    QString getSECTORID();
    QString getMINSTEP();
    QString getPREVWAPRICE();
    QString getFACEUNIT();
    QString getPREVDATE();
    QString getISSUESIZE();
    QString getISIN();
    QString getLATNAME();
    QString getREGNUMBER();
    QString getPREVLEGALCLOSEPRICE();
    QString getCURRENCYID();
    QString getSECTYPE();
    QString getLISTLEVEL();
    QString getSETTLEDATE();

private:
    QString SECID;
    QString BOARDID;
    QString SHORTNAME;
    QString PREVPRICE;
    QString LOTSIZE;
    QString FACEVALUE;
    QString STATUS;
    QString BOARDNAME;
    QString DECIMALS;
    QString SECNAME;
    QString REMARKS;
    QString MARKETCODE;
    QString INSTRID;
    QString SECTORID;
    QString MINSTEP;
    QString PREVWAPRICE;
    QString FACEUNIT;
    QString PREVDATE;
    QString ISSUESIZE;
    QString ISIN;
    QString LATNAME;
    QString REGNUMBER;
    QString PREVLEGALCLOSEPRICE;
    QString CURRENCYID;
    QString SECTYPE;
    QString LISTLEVEL;
    QString SETTLEDATE;
};

#endif // EXCHANGE_DATA_H
