#include "exch_tab.h"

exch_tab::exch_tab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::exch_tab)
{
    ui->setupUi(this);
}

exch_tab::~exch_tab()
{
    delete ui;
}

