/********************************************************************************
** Form generated from reading UI file 'sperinvestwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPERINVESTWINDOW_H
#define UI_SPERINVESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SperInvestWindow
{
public:
    QWidget *tab;
    QTextEdit *textEdit;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *tab_4;

    void setupUi(QTabWidget *SperInvestWindow)
    {
        if (SperInvestWindow->objectName().isEmpty())
            SperInvestWindow->setObjectName("SperInvestWindow");
        SperInvestWindow->resize(640, 480);
        SperInvestWindow->setTabPosition(QTabWidget::South);
        tab = new QWidget();
        tab->setObjectName("tab");
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(150, 20, 471, 31));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(10, 70, 120, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 118, 319));
        scrollArea->setWidget(scrollAreaWidgetContents);
        scrollArea_2 = new QScrollArea(tab);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setGeometry(QRect(149, 69, 471, 321));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 469, 319));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        SperInvestWindow->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        SperInvestWindow->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        SperInvestWindow->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        SperInvestWindow->addTab(tab_4, QString());

        retranslateUi(SperInvestWindow);

        SperInvestWindow->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SperInvestWindow);
    } // setupUi

    void retranslateUi(QTabWidget *SperInvestWindow)
    {
        SperInvestWindow->setWindowTitle(QCoreApplication::translate("SperInvestWindow", "SperInvest", nullptr));
        SperInvestWindow->setTabText(SperInvestWindow->indexOf(tab), QCoreApplication::translate("SperInvestWindow", "\320\223\320\273\320\260\320\262\320\275\320\260\321\217", nullptr));
        SperInvestWindow->setTabText(SperInvestWindow->indexOf(tab_2), QCoreApplication::translate("SperInvestWindow", "\320\235\320\276\320\262\320\276\321\201\321\202\320\270", nullptr));
        SperInvestWindow->setTabText(SperInvestWindow->indexOf(tab_3), QCoreApplication::translate("SperInvestWindow", "\320\221\320\270\321\200\320\266\320\260", nullptr));
        SperInvestWindow->setTabText(SperInvestWindow->indexOf(tab_4), QCoreApplication::translate("SperInvestWindow", "\320\237\321\200\320\276\321\207\320\265\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SperInvestWindow: public Ui_SperInvestWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPERINVESTWINDOW_H
