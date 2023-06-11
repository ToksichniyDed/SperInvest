#ifndef MARKETDATA_INFO_H
#define MARKETDATA_INFO_H

#include <QString>


class marketdata_info
{
private:
    QString SECID;
    QString BOARDID;
    QString BID;
    QString BIDDEPTH;
    QString OFFER;
    QString OFFERDEPTH;
    QString SPREAD;
    QString BIDDEPTHT;
    QString OFFERDEPTHT;
    QString OPEN;
    QString LOW;
    QString HIGH;
    QString LAST;
    QString LASTCHANGE;
    QString LASTCHANGEPRCNT;
    QString QTY;
    QString VALUE;
    QString VALUE_USD;
    QString WAPRICE;
    QString LASTCNGTOLASTWAPRICE;
    QString WAPTOPREVWAPRICEPRCNT;
    QString WAPTOPREVWAPRICE;
    QString CLOSEPRICE;
    QString MARKETPRICETODAY;
    QString MARKETPRICE;
    QString LASTTOPREVPRICE;
    QString NUMTRADES;
    QString VOLTODAY;
    QString VALTODAY;
    QString VALTODAY_USD;
    QString ETFSETTLEPRICE;
    QString TRADINGSTATUS;
    QString UPDATETIME;
    QString LASTBID;
    QString LASTOFFER;
    QString LCLOSEPRICE;
    QString LCURRENTPRICE;
    QString MARKETPRICE2;
    QString NUMBIDS;
    QString NUMOFFERS;
    QString CHANGE;
    QString TIME;
    QString HIGHBID;
    QString LOWOFFER;
    QString PRICEMINUSPREVWAPRICE;
    QString OPENPERIODPRICE;
    QString SEQNUM;
    QString SYSTIME;
    QString CLOSINGAUCTIONPRICE;
    QString CLOSINGAUCTIONVOLUME;
    QString ISSUECAPITALIZATION;
    QString ISSUECAPITALIZATION_UPDATETIME;
    QString ETFSETTLECURRENCY;
    QString VALTODAY_RUR;
    QString TRADINGSESSION;

public:
    marketdata_info( QString secid = " ",  QString boardid = " ",  QString bid = " ",
                     QString biddepth = " ",  QString offer = " ",  QString offerdepth = " ",
                     QString spread = " ",  QString biddeptht = " ",  QString offerdeptht = " ",
                     QString open = " ",  QString low = " ",  QString high = " ",
                     QString last = " ",  QString lastchange = " ",  QString lastchangeprcnt = " ",
                     QString qty = " ",  QString value = " ",  QString value_usd = " ",
                     QString waprice = " ",  QString lastcngtolastwaprice = " ",
                     QString waptoprevwapriceprcnt = " ",  QString waptoprevwaprice = " ",
                     QString closeprice = " ",  QString marketpricetoday = " ",
                     QString marketprice = " ",  QString lasttoprevprice = " ",
                     QString numtrades = " ",  QString voltoday = " ",  QString valtoday = " ",
                     QString valtoday_usd = " ",  QString etfsettleprice = " ",
                     QString tradingstatus = " ",  QString updatetime = " ",
                     QString lastbid = " ",  QString lastoffer = " ",
                     QString lcloseprice = " ",  QString lcurrentprice = " ",
                     QString marketprice2 = " ",  QString numbids = " ",
                     QString numoffers = " ",  QString change = " ",  QString time = " ",
                     QString highbid = " ",  QString lowoffer = " ",
                     QString priceminusprevwaprice = " ",  QString openperiodprice = " ",
                     QString seqnum = " ",  QString systime = " ",
                     QString closingauctionprice = " ",  QString closingauctionvolume = " ",
                     QString issuecapitalization = " ",
                     QString issuecapitalization_updatetime = " ",
                     QString etfsettlecurrency = " ",  QString valtoday_rur = " ",
                     QString tradingsession = " ");
    QString getSECID() ;
    QString getBOARDID() ;
    QString getBID() ;
    QString getBIDDEPTH() ;
    QString getOFFER() ;
    QString getOFFERDEPTH() ;
    QString getSPREAD() ;
    QString getBIDDEPTHT() ;
    QString getOFFERDEPTHT() ;
    QString getOPEN() ;
    QString getLOW() ;
    QString getHIGH() ;
    QString getLAST() ;
    QString getLASTCHANGE() ;
    QString getLASTCHANGEPRCNT() ;
    QString getQTY() ;
    QString getVALUE() ;
    QString getVALUE_USD() ;
    QString getWAPRICE() ;
    QString getLASTCNGTOLASTWAPRICE() ;
    QString getWAPTOPREVWAPRICEPRCNT() ;
    QString getWAPTOPREVWAPRICE() ;
    QString getCLOSEPRICE() ;
    QString getMARKETPRICETODAY() ;
    QString getMARKETPRICE() ;
    QString getLASTTOPREVPRICE() ;
    QString getNUMTRADES() ;
    QString getVOLTODAY() ;
    QString getVALTODAY() ;
    QString getVALTODAY_USD() ;
    QString getETFSETTLEPRICE() ;
    QString getTRADINGSTATUS() ;
    QString getUPDATETIME() ;
    QString getLASTBID() ;
    QString getLASTOFFER() ;
    QString getLCLOSEPRICE() ;
    QString getLCURRENTPRICE() ;
    QString getMARKETPRICE2() ;
    QString getNUMBIDS() ;
    QString getNUMOFFERS() ;
    QString getCHANGE() ;
    QString getTIME() ;
    QString getHIGHBID() ;
    QString getLOWOFFER() ;
    QString getPRICEMINUSPREVWAPRICE() ;
    QString getOPENPERIODPRICE() ;
    QString getSEQNUM() ;
    QString getSYSTIME() ;
    QString getCLOSINGAUCTIONPRICE() ;
    QString getCLOSINGAUCTIONVOLUME() ;
    QString getISSUECAPITALIZATION() ;
    QString getISSUECAPITALIZATION_UPDATETIME() ;
    QString getETFSETTLECURRENCY() ;
    QString getVALTODAY_RUR() ;
    QString getTRADINGSESSION() ;

    void setSECID( QString value);
    void setBOARDID( QString value);
    void setBID( QString value);
    void setBIDDEPTH( QString value);
    void setOFFER( QString value);
    void setOFFERDEPTH( QString value);
    void setSPREAD( QString value);
    void setBIDDEPTHT( QString value);
    void setOFFERDEPTHT( QString value);
    void setOPEN( QString value);
    void setLOW( QString value);
    void setHIGH( QString value);
    void setLAST( QString value);
    void setLASTCHANGE( QString value);
    void setLASTCHANGEPRCNT( QString value);
    void setQTY( QString value);
    void setVALUE( QString value);
    void setVALUE_USD( QString value);
    void setWAPRICE( QString value);
    void setLASTCNGTOLASTWAPRICE( QString value);
    void setWAPTOPREVWAPRICEPRCNT( QString value);
    void setWAPTOPREVWAPRICE( QString value);
    void setCLOSEPRICE( QString value);
    void setMARKETPRICETODAY( QString value);
    void setMARKETPRICE( QString value);
    void setLASTTOPREVPRICE( QString value);
    void setNUMTRADES( QString value);
    void setVOLTODAY( QString value);
    void setVALTODAY( QString value);
    void setVALTODAY_USD( QString value);
    void setETFSETTLEPRICE( QString value);
    void setTRADINGSTATUS( QString value);
    void setUPDATETIME( QString value);
    void setLASTBID( QString value);
    void setLASTOFFER( QString value);
    void setLCLOSEPRICE( QString value);
    void setLCURRENTPRICE( QString value);
    void setMARKETPRICE2( QString value);
    void setNUMBIDS( QString value);
    void setNUMOFFERS( QString value);
    void setCHANGE( QString value);
    void setTIME( QString value);
    void setHIGHBID( QString value);
    void setLOWOFFER( QString value);
    void setPRICEMINUSPREVWAPRICE( QString value);
    void setOPENPERIODPRICE( QString value);
    void setSEQNUM( QString value);
    void setSYSTIME( QString value);
    void setCLOSINGAUCTIONPRICE( QString value);
    void setCLOSINGAUCTIONVOLUME( QString value);
    void setISSUECAPITALIZATION( QString value);
    void setISSUECAPITALIZATION_UPDATETIME( QString value);
    void setETFSETTLECURRENCY( QString value);
    void setVALTODAY_RUR( QString value);
    void setTRADINGSESSION( QString value);


};

#endif // MARKETDATA_INFO_H
