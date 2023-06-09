#ifndef ACCOUNT_INFO_H
#define ACCOUNT_INFO_H
#include <QObject>

class account_info
{
public:
    account_info(QString account_id = "",QString account_name= "", QString account_balance= "",QString currency= "", QString open_date= "", QString status= "",QString created_at= "",QString tariff_plan= "");
    QString get_account_name();
    QString get_account_id();
    QString get_account_balance();
    QString get_currency();
    QString get_open_date();
    QString get_status();
    QString get_created_at();
    QString get_tariff_plan();

private:
    QString account_name;
    QString account_id;
    QString account_balance;
    QString currency;
    QString open_date;
    QString status;
    QString created_at;
    QString tariff_plan;
};

#endif // ACCOUNT_INFO_H
