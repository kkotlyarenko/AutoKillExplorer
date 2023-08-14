#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCloseEvent>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddButton_clicked();

    void on_RemoveButton_clicked();

    void on_StartButton_clicked();

    //tray
    void createAction();
    void createTrayIcon();
    void closeEvent(QCloseEvent *);
    void onSystemTrayLeftClicked(QSystemTrayIcon::ActivationReason Reason);

private:
    Ui::MainWindow *ui;

    QSystemTrayIcon *myTray;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    std::atomic<bool> program_is_running{ false };
    std::atomic<bool> vector_changed{ true };
    std::atomic<unsigned int> update_interval_millisecs{ 10000 };
};
#endif // MAINWINDOW_H
