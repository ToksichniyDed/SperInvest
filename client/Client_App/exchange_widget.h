#ifndef EXCHANGE_WIDGET_H
#define EXCHANGE_WIDGET_H

#include <QWidget>
#include <QTimer>

#include "ui_exchange_data.h"
#include "exchange_data.h"
#include "client.h"
#include "marketdata_info.h"
#include "buy_exchange.h"

namespace Ui {
class exchange_widget;
}

class exchange_widget : public QWidget
{
    Q_OBJECT

public:
    explicit exchange_widget(QWidget *parent = nullptr,Client* ex_client = nullptr,exchange_data* exch_info = nullptr,marketdata_info* marketdata_info1 = nullptr);
    ~exchange_widget();
    void setExchangeData();
    void setMarketdataData(marketdata_info* marketdata_info2);
    void onBuy_Exch_ButtonClicked();//Покупка акций

private:
    Client* ex_client;
    exchange_data* exch_info;
    Ui::exchange_widget *ui;
    marketdata_info* marketdata_info1;
};

#endif // EXCHANGE_WIDGET_H
