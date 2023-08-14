/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QListWidget *listCurrentApps;
    QLineEdit *AddTextEdit;
    QPushButton *AddButton;
    QPushButton *RemoveButton;
    QPushButton *StartButton;
    QDoubleSpinBox *IntervalCheck;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(400, 330);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(400, 330));
        MainWindow->setMaximumSize(QSize(400, 330));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        listCurrentApps = new QListWidget(centralwidget);
        listCurrentApps->setObjectName("listCurrentApps");
        listCurrentApps->setGeometry(QRect(10, 10, 381, 201));
        AddTextEdit = new QLineEdit(centralwidget);
        AddTextEdit->setObjectName("AddTextEdit");
        AddTextEdit->setGeometry(QRect(10, 210, 221, 31));
        AddTextEdit->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        AddTextEdit->setInputMethodHints(Qt::ImhNone);
 	AddTextEdit->setMaxLength(100);
        AddButton = new QPushButton(centralwidget);
        AddButton->setObjectName("AddButton");
        AddButton->setGeometry(QRect(230, 210, 101, 31));
        RemoveButton = new QPushButton(centralwidget);
        RemoveButton->setObjectName("RemoveButton");
        RemoveButton->setGeometry(QRect(330, 210, 61, 31));
        StartButton = new QPushButton(centralwidget);
        StartButton->setObjectName("StartButton");
        StartButton->setGeometry(QRect(10, 280, 381, 41));
        IntervalCheck = new QDoubleSpinBox(centralwidget);
        IntervalCheck->setObjectName("IntervalCheck");
        IntervalCheck->setGeometry(QRect(330, 250, 62, 22));
        IntervalCheck->setDecimals(1);
        IntervalCheck->setSingleStep(0.500000000000000);
        IntervalCheck->setValue(3.000000000000000);
	IntervalCheck->setMinimum(0.500000000000000);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 250, 311, 21));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "AutoKillExplorer", nullptr));
#if QT_CONFIG(tooltip)
        AddTextEdit->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        AddTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Write the name of the app process", nullptr));
        AddButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        RemoveButton->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        StartButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Select check interval, seconds:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
