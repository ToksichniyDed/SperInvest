#ifndef EXCH_TAB_H
#define EXCH_TAB_H

#include <QTabWidget>

#include "client.h"
#include "exch.h"
#include "ui_exch_tab.h"

namespace Ui {
class exch_tab;
}

class exch_tab : public QTabWidget
{
    Q_OBJECT

public:
    explicit exch_tab(QWidget *parent = nullptr);
    ~exch_tab();

private:
    Ui::exch_tab *ui;
};

#endif // EXCH_TAB_H
