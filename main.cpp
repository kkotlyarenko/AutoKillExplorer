//Made by kkotlyarenko
#include "mainwindow.h"
#include <QApplication>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLockFile lockFile(QDir::temp().absoluteFilePath("lurity.lock"));
    if (!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Application is already running");
        msgBox.exec();
        return 1;
    }
    MainWindow w;
    //w.show();
    return a.exec();
}
