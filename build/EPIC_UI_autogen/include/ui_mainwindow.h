/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *lblProgramName;
    QLabel *lblParameter1;
    QPlainTextEdit *plainTextEdit;
    QComboBox *comboBox;
    QPushButton *btnCreateCommand;
    QTextBrowser *textDisplayCommands;
    QLabel *label_3;
    QPushButton *btnClearCommands;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(60, 40, 271, 61));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lblProgramName = new QLabel(gridLayoutWidget);
        lblProgramName->setObjectName("lblProgramName");

        gridLayout->addWidget(lblProgramName, 0, 0, 1, 1);

        lblParameter1 = new QLabel(gridLayoutWidget);
        lblParameter1->setObjectName("lblParameter1");

        gridLayout->addWidget(lblParameter1, 1, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit->setObjectName("plainTextEdit");

        gridLayout->addWidget(plainTextEdit, 0, 1, 1, 1);

        comboBox = new QComboBox(gridLayoutWidget);
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        btnCreateCommand = new QPushButton(centralwidget);
        btnCreateCommand->setObjectName("btnCreateCommand");
        btnCreateCommand->setGeometry(QRect(320, 310, 141, 26));
        textDisplayCommands = new QTextBrowser(centralwidget);
        textDisplayCommands->setObjectName("textDisplayCommands");
        textDisplayCommands->setGeometry(QRect(70, 410, 611, 71));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(70, 380, 151, 18));
        btnClearCommands = new QPushButton(centralwidget);
        btnClearCommands->setObjectName("btnClearCommands");
        btnClearCommands->setGeometry(QRect(590, 490, 91, 26));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "EPIC UI", nullptr));
        lblProgramName->setText(QCoreApplication::translate("MainWindow", "Program Name", nullptr));
        lblParameter1->setText(QCoreApplication::translate("MainWindow", "Parameter 1", nullptr));
        btnCreateCommand->setText(QCoreApplication::translate("MainWindow", "Create Command", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", " SLURM COMMANDS", nullptr));
        btnClearCommands->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
