#ifndef BUY_EXCHANGE_H
#define BUY_EXCHANGE_H

#include <QDialog>
#include <QMessageBox>

#include "client.h"
#include "ui_buy_exchange.h"
#include "exchange_data.h"
#include "marketdata_info.h"

namespace Ui {
class buy_exchange;
}

class buy_exchange : public QDialog
{
    Q_OBJECT

public:
    explicit buy_exchange(Client* buy_client = nullptr,QString secid = "", float result_sum = 0.0,float price = 0.0, QWidget *parent = nullptr);
    ~buy_exchange();
    void receivePurchaseExchange(QString message);

private:
    Ui::buy_exchange *ui;
    Client* buy_client;
    QString secid;
    float result_sum;
    float price;

signals:
    void send_buy_exchange_information(const QString& message);

public slots:
    void onFirstLineEditTextChanged(const QString& text);
    void onBuyButton();
};

#endif // BUY_EXCHANGE_H
