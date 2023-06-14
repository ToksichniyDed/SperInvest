#ifndef EXCH_H
#define EXCH_H

#include <QWidget>

#include "client.h"
#include "marketdata_info.h"
#include "ui_exch.h"
#include "purchase.h"
#include "exchange_data.h"

namespace Ui {
class exch;
}

class exch : public QWidget
{
    Q_OBJECT

public:
    explicit exch(Client* exc_client = nullptr,QString acc_id = "",purchase* purch = nullptr, QWidget *parent = nullptr);
    ~exch();
    void Show_Info();

private:
    Ui::exch *ui;
    Client* exc_client;
    QString acc_id;
    purchase* purch;
};

#endif // EXCH_H
