#include "exchange_data.h"

exchange_data::exchange_data(QString SECID, QString BOARDID, QString SHORTNAME, QString PREVPRICE, QString LOTSIZE, QString FACEVALUE, QString STATUS, QString BOARDNAME, QString DECIMALS, QString SECNAME, QString REMARKS, QString MARKETCODE, QString INSTRID, QString SECTORID, QString MINSTEP, QString PREVWAPRICE, QString FACEUNIT, QString PREVDATE, QString ISSUESIZE, QString ISIN, QString LATNAME, QString REGNUMBER, QString PREVLEGALCLOSEPRICE, QString CURRENCYID, QString SECTYPE, QString LISTLEVEL, QString SETTLEDATE)
    : SECID(SECID), BOARDID(BOARDID), SHORTNAME(SHORTNAME), PREVPRICE(PREVPRICE), LOTSIZE(LOTSIZE), FACEVALUE(FACEVALUE),
    STATUS(STATUS), BOARDNAME(BOARDNAME), DECIMALS(DECIMALS), SECNAME(SECNAME), REMARKS(REMARKS), MARKETCODE(MARKETCODE),
    INSTRID(INSTRID), SECTORID(SECTORID), MINSTEP(MINSTEP), PREVWAPRICE(PREVWAPRICE), FACEUNIT(FACEUNIT), PREVDATE(PREVDATE),
    ISSUESIZE(ISSUESIZE), ISIN(ISIN), LATNAME(LATNAME), REGNUMBER(REGNUMBER), PREVLEGALCLOSEPRICE(PREVLEGALCLOSEPRICE),
    CURRENCYID(CURRENCYID), SECTYPE(SECTYPE), LISTLEVEL(LISTLEVEL), SETTLEDATE(SETTLEDATE)
{

}

QString exchange_data::getSECID() { return SECID; }

QString exchange_data::getBOARDID() { return BOARDID; }

QString exchange_data::getSHORTNAME() { return SHORTNAME; }

QString exchange_data::getPREVPRICE() { return PREVPRICE; }

QString exchange_data::getLOTSIZE() { return LOTSIZE; }

QString exchange_data::getFACEVALUE() { return FACEVALUE; }

QString exchange_data::getSTATUS() { return STATUS; }

QString exchange_data::getBOARDNAME() { return BOARDNAME; }

QString exchange_data::getDECIMALS() { return DECIMALS; }

QString exchange_data::getSECNAME() { return SECNAME; }

QString exchange_data::getREMARKS() { return REMARKS; }

QString exchange_data::getMARKETCODE() { return MARKETCODE; }

QString exchange_data::getINSTRID() { return INSTRID; }

QString exchange_data::getSECTORID() { return SECTORID; }

QString exchange_data::getMINSTEP() { return MINSTEP; }

QString exchange_data::getPREVWAPRICE() { return PREVWAPRICE; }

QString exchange_data::getFACEUNIT() { return FACEUNIT; }

QString exchange_data::getPREVDATE() { return PREVDATE; }

QString exchange_data::getISSUESIZE() { return ISSUESIZE; }

QString exchange_data::getISIN() { return ISIN; }

QString exchange_data::getLATNAME() { return LATNAME; }

QString exchange_data::getREGNUMBER() { return REGNUMBER; }

QString exchange_data::getPREVLEGALCLOSEPRICE() { return PREVLEGALCLOSEPRICE; }

QString exchange_data::getCURRENCYID() { return CURRENCYID; }

QString exchange_data::getSECTYPE() { return SECTYPE; }

QString exchange_data::getLISTLEVEL() { return LISTLEVEL; }

QString exchange_data::getSETTLEDATE() { return SETTLEDATE; }
