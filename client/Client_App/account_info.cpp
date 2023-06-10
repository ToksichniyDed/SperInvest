#include "account_info.h"

account_info::account_info(QString account_id,QString account_name, QString account_balance,QString currency, QString open_date, QString status,QString created_at,QString tariff_plan)
    :account_id(account_id), account_name(account_name), account_balance(account_balance),currency(currency),open_date(open_date),status(status),created_at(created_at),tariff_plan(tariff_plan)
{

}

QString account_info::get_account_name()
{
    return account_name;
}

QString account_info::get_account_id()
{
    return account_id;
}

QString account_info::get_account_balance()
{
    return account_balance;
}

QString account_info::get_currency()
{
    return currency;
}

QString account_info::get_open_date()
{
    return open_date;
}

QString account_info::get_status()
{
    return status;
}

QString account_info::get_created_at()
{
    return created_at;
}

QString account_info::get_tariff_plan()
{
    return tariff_plan;
}
