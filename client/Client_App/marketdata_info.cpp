#include "marketdata_info.h"

marketdata_info::marketdata_info(QString secid,  QString boardid,  QString bid,
                                 QString biddepth,  QString offer,  QString offerdepth,
                                 QString spread,  QString biddeptht,  QString offerdeptht,
                                 QString open,  QString low,  QString high,
                                 QString last,  QString lastchange,  QString lastchangeprcnt,
                                 QString qty,  QString value,  QString value_usd,
                                 QString waprice,  QString lastcngtolastwaprice,
                                 QString waptoprevwapriceprcnt,  QString waptoprevwaprice,
                                 QString closeprice,  QString marketpricetoday,
                                 QString marketprice,  QString lasttoprevprice,
                                 QString numtrades,  QString voltoday,  QString valtoday,
                                 QString valtoday_usd,  QString etfsettleprice,
                                 QString tradingstatus,  QString updatetime,  QString lastbid,
                                 QString lastoffer,  QString lcloseprice,
                                 QString lcurrentprice,  QString marketprice2,
                                 QString numbids,  QString numoffers,  QString change,
                                 QString time,  QString highbid,  QString lowoffer,
                                 QString priceminusprevwaprice,  QString openperiodprice,
                                 QString seqnum,  QString systime,  QString closingauctionprice,
                                 QString closingauctionvolume,  QString issuecapitalization,
                                 QString issuecapitalization_updatetime,  QString etfsettlecurrency,
                                 QString valtoday_rur,  QString tradingsession)
    : SECID(secid),
    BOARDID(boardid),
    BID(bid),
    BIDDEPTH(biddepth),
    OFFER(offer),
    OFFERDEPTH(offerdepth),
    SPREAD(spread),
    BIDDEPTHT(biddeptht),
    OFFERDEPTHT(offerdeptht),
    OPEN(open),
    LOW(low),
    HIGH(high),
    LAST(last),
    LASTCHANGE(lastchange),
    LASTCHANGEPRCNT(lastchangeprcnt),
    QTY(qty),
    VALUE(value),
    VALUE_USD(value_usd),
    WAPRICE(waprice),
    LASTCNGTOLASTWAPRICE(lastcngtolastwaprice),
    WAPTOPREVWAPRICEPRCNT(waptoprevwapriceprcnt),
    WAPTOPREVWAPRICE(waptoprevwaprice),
    CLOSEPRICE(closeprice),
    MARKETPRICETODAY(marketpricetoday),
    MARKETPRICE(marketprice),
    LASTTOPREVPRICE(lasttoprevprice),
    NUMTRADES(numtrades),
    VOLTODAY(voltoday),
    VALTODAY(valtoday),
    VALTODAY_USD(valtoday_usd),
    ETFSETTLEPRICE(etfsettleprice),
    TRADINGSTATUS(tradingstatus),
    UPDATETIME(updatetime),
    LASTBID(lastbid),
    LASTOFFER(lastoffer),
    LCLOSEPRICE(lcloseprice),
    LCURRENTPRICE(lcurrentprice),
    MARKETPRICE2(marketprice2),
    NUMBIDS(numbids),
    NUMOFFERS(numoffers),
    CHANGE(change),
    TIME(time),
    HIGHBID(highbid),
    LOWOFFER(lowoffer),
    PRICEMINUSPREVWAPRICE(priceminusprevwaprice),
    OPENPERIODPRICE(openperiodprice),
    SEQNUM(seqnum),
    SYSTIME(systime),
    CLOSINGAUCTIONPRICE(closingauctionprice),
    CLOSINGAUCTIONVOLUME(closingauctionvolume),
    ISSUECAPITALIZATION(issuecapitalization),
    ISSUECAPITALIZATION_UPDATETIME(issuecapitalization_updatetime),
    ETFSETTLECURRENCY(etfsettlecurrency),
    VALTODAY_RUR(valtoday_rur),
    TRADINGSESSION(tradingsession)
{

}

QString marketdata_info::getSECID()
{
    return SECID;
}

QString marketdata_info::getBOARDID()
{
    return BOARDID;
}

QString marketdata_info::getBID()
{
    return BID;
}

QString marketdata_info::getBIDDEPTH()
{
    return BIDDEPTH;
}

QString marketdata_info::getOFFER()
{
    return OFFER;
}

QString marketdata_info::getOFFERDEPTH()
{
    return OFFERDEPTH;
}

QString marketdata_info::getSPREAD()
{
    return SPREAD;
}

QString marketdata_info::getBIDDEPTHT()
{
    return BIDDEPTHT;
}

QString marketdata_info::getOFFERDEPTHT()
{
    return OFFERDEPTHT;
}

QString marketdata_info::getOPEN()
{
    return OPEN;
}

QString marketdata_info::getLOW()
{
    return LOW;
}

QString marketdata_info::getHIGH()
{
    return HIGH;
}

QString marketdata_info::getLAST()
{
    return LAST;
}

QString marketdata_info::getLASTCHANGE()
{
    return LASTCHANGE;
}

QString marketdata_info::getLASTCHANGEPRCNT()
{
    return LASTCHANGEPRCNT;
}

QString marketdata_info::getQTY()
{
    return QTY;
}

QString marketdata_info::getVALUE()
{
    return VALUE;
}

QString marketdata_info::getVALUE_USD()
{
    return VALUE_USD;
}

QString marketdata_info::getWAPRICE()
{
    return WAPRICE;
}

QString marketdata_info::getLASTCNGTOLASTWAPRICE()
{
    return LASTCNGTOLASTWAPRICE;
}

QString marketdata_info::getWAPTOPREVWAPRICEPRCNT()
{
    return WAPTOPREVWAPRICEPRCNT;
}

QString marketdata_info::getWAPTOPREVWAPRICE()
{
    return WAPTOPREVWAPRICE;
}

QString marketdata_info::getCLOSEPRICE()
{
    return CLOSEPRICE;
}

QString marketdata_info::getMARKETPRICETODAY()
{
    return MARKETPRICETODAY;
}

QString marketdata_info::getMARKETPRICE()
{
    return MARKETPRICE;
}

QString marketdata_info::getLASTTOPREVPRICE()
{
    return LASTTOPREVPRICE;
}

QString marketdata_info::getNUMTRADES()
{
    return NUMTRADES;
}

QString marketdata_info::getVOLTODAY()
{
    return VOLTODAY;
}

QString marketdata_info::getVALTODAY()
{
    return VALTODAY;
}

QString marketdata_info::getVALTODAY_USD()
{
    return VALTODAY_USD;
}

QString marketdata_info::getETFSETTLEPRICE()
{
    return ETFSETTLEPRICE;
}

QString marketdata_info::getTRADINGSTATUS()
{
    return TRADINGSTATUS;
}

QString marketdata_info::getUPDATETIME()
{
    return UPDATETIME;
}

QString marketdata_info::getLASTBID()
{
    return LASTBID;
}

QString marketdata_info::getLASTOFFER()
{
    return LASTOFFER;
}

QString marketdata_info::getLCLOSEPRICE()
{
    return LCLOSEPRICE;
}

QString marketdata_info::getLCURRENTPRICE()
{
    return LCURRENTPRICE;
}

QString marketdata_info::getMARKETPRICE2()
{
    return MARKETPRICE2;
}

QString marketdata_info::getNUMBIDS()
{
    return NUMBIDS;
}

QString marketdata_info::getNUMOFFERS()
{
    return NUMOFFERS;
}

QString marketdata_info::getCHANGE()
{
    return CHANGE;
}

QString marketdata_info::getTIME()
{
    return TIME;
}

QString marketdata_info::getHIGHBID()
{
    return HIGHBID;
}

QString marketdata_info::getLOWOFFER()
{
    return LOWOFFER;
}

QString marketdata_info::getPRICEMINUSPREVWAPRICE()
{
    return PRICEMINUSPREVWAPRICE;
}

QString marketdata_info::getOPENPERIODPRICE()
{
    return OPENPERIODPRICE;
}

QString marketdata_info::getSEQNUM()
{
    return SEQNUM;
}

QString marketdata_info::getSYSTIME()
{
    return SYSTIME;
}

QString marketdata_info::getCLOSINGAUCTIONPRICE()
{
    return CLOSINGAUCTIONPRICE;
}

QString marketdata_info::getCLOSINGAUCTIONVOLUME()
{
    return CLOSINGAUCTIONVOLUME;
}

QString marketdata_info::getISSUECAPITALIZATION()
{
    return ISSUECAPITALIZATION;
}

QString marketdata_info::getISSUECAPITALIZATION_UPDATETIME()
{
    return ISSUECAPITALIZATION_UPDATETIME;
}

QString marketdata_info::getETFSETTLECURRENCY()
{
    return ETFSETTLECURRENCY;
}

QString marketdata_info::getVALTODAY_RUR()
{
    return VALTODAY_RUR;
}

QString marketdata_info::getTRADINGSESSION()
{
    return TRADINGSESSION;
}

void marketdata_info::setSECID( QString value)
{
    SECID = value;
}

void marketdata_info::setBOARDID( QString value)
{
    BOARDID = value;
}

void marketdata_info::setBID( QString value)
{
    BID = value;
}

void marketdata_info::setBIDDEPTH( QString value)
{
    BIDDEPTH = value;
}

void marketdata_info::setOFFER( QString value)
{
    OFFER = value;
}

void marketdata_info::setOFFERDEPTH( QString value)
{
    OFFERDEPTH = value;
}

void marketdata_info::setSPREAD( QString value)
{
    SPREAD = value;
}

void marketdata_info::setBIDDEPTHT( QString value)
{
    BIDDEPTHT = value;
}

void marketdata_info::setOFFERDEPTHT( QString value)
{
    OFFERDEPTHT = value;
}

void marketdata_info::setOPEN( QString value)
{
    OPEN = value;
}

void marketdata_info::setLOW( QString value)
{
    LOW = value;
}

void marketdata_info::setHIGH( QString value)
{
    HIGH = value;
}

void marketdata_info::setLAST( QString value)
{
    LAST = value;
}

void marketdata_info::setLASTCHANGE( QString value)
{
    LASTCHANGE = value;
}

void marketdata_info::setLASTCHANGEPRCNT( QString value)
{
    LASTCHANGEPRCNT = value;
}

void marketdata_info::setQTY( QString value)
{
    QTY = value;
}

void marketdata_info::setVALUE( QString value)
{
    VALUE = value;
}

void marketdata_info::setVALUE_USD( QString value)
{
    VALUE_USD = value;
}

void marketdata_info::setWAPRICE( QString value)
{
    WAPRICE = value;
}

void marketdata_info::setLASTCNGTOLASTWAPRICE( QString value)
{
    LASTCNGTOLASTWAPRICE = value;
}

void marketdata_info::setWAPTOPREVWAPRICEPRCNT( QString value)
{
    WAPTOPREVWAPRICEPRCNT = value;
}

void marketdata_info::setWAPTOPREVWAPRICE( QString value)
{
    WAPTOPREVWAPRICE = value;
}

void marketdata_info::setCLOSEPRICE( QString value)
{
    CLOSEPRICE = value;
}

void marketdata_info::setMARKETPRICETODAY( QString value)
{
    MARKETPRICETODAY = value;
}

void marketdata_info::setMARKETPRICE( QString value)
{
    MARKETPRICE = value;
}

void marketdata_info::setLASTTOPREVPRICE( QString value)
{
    LASTTOPREVPRICE = value;
}

void marketdata_info::setNUMTRADES( QString value)
{
    NUMTRADES = value;
}

void marketdata_info::setVOLTODAY( QString value)
{
    VOLTODAY = value;
}

void marketdata_info::setVALTODAY( QString value)
{
    VALTODAY = value;
}

void marketdata_info::setVALTODAY_USD( QString value)
{
    VALTODAY_USD = value;
}

void marketdata_info::setETFSETTLEPRICE( QString value)
{
    ETFSETTLEPRICE = value;
}

void marketdata_info::setTRADINGSTATUS( QString value)
{
    TRADINGSTATUS = value;
}

void marketdata_info::setUPDATETIME( QString value)
{
    UPDATETIME = value;
}

void marketdata_info::setLASTBID( QString value)
{
    LASTBID = value;
}

void marketdata_info::setLASTOFFER( QString value)
{
    LASTOFFER = value;
}

void marketdata_info::setLCLOSEPRICE( QString value)
{
    LCLOSEPRICE = value;
}

void marketdata_info::setLCURRENTPRICE( QString value)
{
    LCURRENTPRICE = value;
}

void marketdata_info::setMARKETPRICE2( QString value)
{
    MARKETPRICE2 = value;
}

void marketdata_info::setNUMBIDS( QString value)
{
    NUMBIDS = value;
}

void marketdata_info::setNUMOFFERS( QString value)
{
    NUMOFFERS = value;
}

void marketdata_info::setCHANGE( QString value)
{
    CHANGE = value;
}

void marketdata_info::setTIME( QString value)
{
    TIME = value;
}

void marketdata_info::setHIGHBID( QString value)
{
    HIGHBID = value;
}

void marketdata_info::setLOWOFFER( QString value)
{
    LOWOFFER = value;
}

void marketdata_info::setPRICEMINUSPREVWAPRICE( QString value)
{
    PRICEMINUSPREVWAPRICE = value;
}

void marketdata_info::setOPENPERIODPRICE( QString value)
{
    OPENPERIODPRICE = value;
}

void marketdata_info::setSEQNUM( QString value)
{
    SEQNUM = value;
}

void marketdata_info::setSYSTIME( QString value)
{
    SYSTIME = value;
}

void marketdata_info::setCLOSINGAUCTIONPRICE( QString value)
{
    CLOSINGAUCTIONPRICE = value;
}

void marketdata_info::setCLOSINGAUCTIONVOLUME( QString value)
{
    CLOSINGAUCTIONVOLUME = value;
}

void marketdata_info::setISSUECAPITALIZATION( QString value)
{
    ISSUECAPITALIZATION = value;
}

void marketdata_info::setISSUECAPITALIZATION_UPDATETIME( QString value)
{
    ISSUECAPITALIZATION_UPDATETIME = value;
}

void marketdata_info::setETFSETTLECURRENCY( QString value)
{
    ETFSETTLECURRENCY = value;
}

void marketdata_info::setVALTODAY_RUR( QString value)
{
    VALTODAY_RUR = value;
}

void marketdata_info::setTRADINGSESSION( QString value)
{
    TRADINGSESSION = value;
}
