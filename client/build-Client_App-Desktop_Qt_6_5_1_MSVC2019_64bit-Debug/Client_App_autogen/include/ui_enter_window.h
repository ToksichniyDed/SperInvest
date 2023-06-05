/********************************************************************************
** Form generated from reading UI file 'enter_window.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTER_WINDOW_H
#define UI_ENTER_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_enter_window
{
public:
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *log_button;
    QPushButton *reg_button;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *login;
    QLineEdit *password;

    void setupUi(QDialog *enter_window)
    {
        if (enter_window->objectName().isEmpty())
            enter_window->setObjectName("enter_window");
        enter_window->resize(565, 404);
        QPalette palette;
        QBrush brush(QColor(0, 0, 99, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        enter_window->setPalette(palette);
        groupBox = new QGroupBox(enter_window);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(90, 60, 371, 241));
        groupBox->setAlignment(Qt::AlignCenter);
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(20, 150, 331, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        log_button = new QPushButton(horizontalLayoutWidget);
        log_button->setObjectName("log_button");

        horizontalLayout->addWidget(log_button);

        reg_button = new QPushButton(horizontalLayoutWidget);
        reg_button->setObjectName("reg_button");

        horizontalLayout->addWidget(reg_button);

        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(90, 90, 191, 51));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        login = new QLineEdit(verticalLayoutWidget);
        login->setObjectName("login");

        verticalLayout->addWidget(login);

        password = new QLineEdit(verticalLayoutWidget);
        password->setObjectName("password");

        verticalLayout->addWidget(password);


        retranslateUi(enter_window);

        QMetaObject::connectSlotsByName(enter_window);
    } // setupUi

    void retranslateUi(QDialog *enter_window)
    {
        enter_window->setWindowTitle(QCoreApplication::translate("enter_window", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        groupBox->setTitle(QString());
        log_button->setText(QCoreApplication::translate("enter_window", "\320\222\321\205\320\276\320\264", nullptr));
        reg_button->setText(QCoreApplication::translate("enter_window", "\320\235\320\265\321\202 \320\260\320\272\320\272\320\260\321\203\320\275\321\202\320\260?", nullptr));
        login->setPlaceholderText(QCoreApplication::translate("enter_window", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        password->setPlaceholderText(QCoreApplication::translate("enter_window", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class enter_window: public Ui_enter_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTER_WINDOW_H
