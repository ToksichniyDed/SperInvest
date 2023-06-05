/********************************************************************************
** Form generated from reading UI file 'registration_window.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATION_WINDOW_H
#define UI_REGISTRATION_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_registration_window
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *Username;
    QLineEdit *Email;
    QLineEdit *Password;
    QLineEdit *Phone;
    QHBoxLayout *horizontalLayout;
    QPushButton *back_button;
    QPushButton *rback_button;

    void setupUi(QDialog *registration_window)
    {
        if (registration_window->objectName().isEmpty())
            registration_window->setObjectName("registration_window");
        registration_window->resize(556, 374);
        verticalLayoutWidget = new QWidget(registration_window);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(70, 90, 421, 136));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Username = new QLineEdit(verticalLayoutWidget);
        Username->setObjectName("Username");

        verticalLayout->addWidget(Username);

        Email = new QLineEdit(verticalLayoutWidget);
        Email->setObjectName("Email");

        verticalLayout->addWidget(Email);

        Password = new QLineEdit(verticalLayoutWidget);
        Password->setObjectName("Password");

        verticalLayout->addWidget(Password);

        Phone = new QLineEdit(verticalLayoutWidget);
        Phone->setObjectName("Phone");

        verticalLayout->addWidget(Phone);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        back_button = new QPushButton(verticalLayoutWidget);
        back_button->setObjectName("back_button");

        horizontalLayout->addWidget(back_button);

        rback_button = new QPushButton(verticalLayoutWidget);
        rback_button->setObjectName("rback_button");

        horizontalLayout->addWidget(rback_button);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(registration_window);

        QMetaObject::connectSlotsByName(registration_window);
    } // setupUi

    void retranslateUi(QDialog *registration_window)
    {
        registration_window->setWindowTitle(QCoreApplication::translate("registration_window", "Dialog", nullptr));
        Username->setPlaceholderText(QCoreApplication::translate("registration_window", "\320\235\320\270\320\272\320\275\320\265\320\271\320\274", nullptr));
        Email->setPlaceholderText(QCoreApplication::translate("registration_window", "Email(\320\273\320\276\320\263\320\270\320\275)", nullptr));
        Password->setPlaceholderText(QCoreApplication::translate("registration_window", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        Phone->setPlaceholderText(QCoreApplication::translate("registration_window", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275\320\275\321\213\320\271 \320\275\320\276\320\274\320\265\321\200", nullptr));
        back_button->setText(QCoreApplication::translate("registration_window", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        rback_button->setText(QCoreApplication::translate("registration_window", "\320\227\320\260\321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class registration_window: public Ui_registration_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATION_WINDOW_H
