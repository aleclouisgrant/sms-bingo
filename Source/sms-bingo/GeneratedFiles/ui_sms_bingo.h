/********************************************************************************
** Form generated from reading UI file 'sms_bingo.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMS_BINGO_H
#define UI_SMS_BINGO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sms_bingoClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *sms_bingoClass)
    {
        if (sms_bingoClass->objectName().isEmpty())
            sms_bingoClass->setObjectName(QString::fromUtf8("sms_bingoClass"));
        sms_bingoClass->resize(600, 400);
        menuBar = new QMenuBar(sms_bingoClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        sms_bingoClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(sms_bingoClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        sms_bingoClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(sms_bingoClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sms_bingoClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(sms_bingoClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sms_bingoClass->setStatusBar(statusBar);

        retranslateUi(sms_bingoClass);

        QMetaObject::connectSlotsByName(sms_bingoClass);
    } // setupUi

    void retranslateUi(QMainWindow *sms_bingoClass)
    {
        sms_bingoClass->setWindowTitle(QApplication::translate("sms_bingoClass", "sms_bingo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sms_bingoClass: public Ui_sms_bingoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMS_BINGO_H
