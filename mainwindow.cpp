//Made by kkotlyarenko
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QMenu>
#include <fstream>
#include <vector>
#include <QProcess>
#include <QDebug>
#include <windows.h>
#include <tlhelp32.h>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include<sstream>

int is_process_running(std::wstring targetProcessName)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //all processes
    PROCESSENTRY32W entry; //current process
    entry.dwSize = sizeof entry;
    if (!Process32FirstW(snap, &entry)) { //start with the first in snapshot
        return 0;
    }
    do {
        if (std::wstring(entry.szExeFile) == targetProcessName) {
            return 1;
        }
    } while (Process32NextW(snap, &entry)); //keep going until end of snapshot
    return 0;
}

std::vector<std::wstring> app_list;
std::mutex my_mutex;

std::vector<std::wstring> read_app_list(){
    my_mutex.lock();
    auto res = app_list;
    my_mutex.unlock();
    return res;
}

void add_app_list(std::wstring wstr){
    my_mutex.lock();
    app_list.emplace_back(wstr);
    my_mutex.unlock();
}

void del_app_list(std::wstring wstr){
    my_mutex.lock();
    app_list.erase(find(app_list.begin(), app_list.end(), wstr));
    my_mutex.unlock();
}

void check_filter(){
    const int MAXLEN = 100;
    std::wifstream iff; iff.open("userdata_ake.json");
    if(!iff) {
        iff.close();
        std::wofstream oFile ( "userdata_ake.json" ) ;
        std::wstring base = L"3.0\nstop\nAtomicHeart-WinGDK-Shipping.exe\nForzaHorizon5.exe";
        oFile.write(base.c_str(), base.size());
        oFile.close();
        int attr = GetFileAttributes(L"userdata_ake.json");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes(L"userdata_ake.json", attr | FILE_ATTRIBUTE_HIDDEN);
        }
    }
    else {
        iff.close();
        std::wifstream in("userdata_ake.json");
        std::wofstream out("outfile.json");
        std::wstring is_expl, inputstr, state_str;

        in >> inputstr;
        in >> state_str;
        double d = 3.0;
        try {
            d = std::stod(inputstr);
        }
        catch (...) {}
        if (d >= 0.5 && d <= 100) {
            out << d;
        } else if (d > 100) {
            out << "100.0";
        } else {
            out << "0.5";
        }
        std::wstring tmp;
        if(inputstr.size() < MAXLEN){
            for (int i = int(inputstr.size()) - 1; i > int(inputstr.size()) - 5 and i > 0; --i) {
                tmp += inputstr[i];
            }
            is_expl = inputstr;
            transform(is_expl.begin(), is_expl.end(),is_expl.begin(),::tolower);
            if (tmp == L"exe." && is_expl != L"explorer.exe") {
                out << '\n' << inputstr;
            }
        }
        if(state_str.size() < MAXLEN && (state_str == L"start" || state_str == L"stop")){
            out << '\n' << state_str;
        }
        else{
            out << L"\nstop";
            tmp = L"";
            if(state_str.size() < MAXLEN){
                for (int i = int(state_str.size()) - 1; i > int(state_str.size()) - 5 and i > 0; --i) {
                    tmp += state_str[i];
                }
                is_expl = state_str;
                transform(is_expl.begin(), is_expl.end(),is_expl.begin(),::tolower);
                if (tmp == L"exe." && is_expl != L"explorer.exe") {
                    out << '\n' << state_str;
                }
            }
        }
        while (in >> inputstr) {
            tmp = L"";
            if(inputstr.size() < MAXLEN){
                for (int i = int(inputstr.size()) - 1; i > int(inputstr.size()) - 5 and i > 0; --i) {
                    tmp += inputstr[i];
                }
                is_expl = inputstr;
                transform(is_expl.begin(), is_expl.end(),is_expl.begin(),::tolower);
                if (tmp == L"exe." && is_expl != L"explorer.exe") {
                    out << '\n' << inputstr;
                }
            }
        }
        in.close();
        out.close();
        remove("userdata_ake.json");
        rename("outfile.json", "userdata_ake.json");
        int attr = GetFileAttributes(L"userdata_ake.json");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes(L"userdata_ake.json", attr | FILE_ATTRIBUTE_HIDDEN);
        }
    }
}

void run_autokillexplorer( std::atomic<bool>& program_is_running, std::atomic<unsigned int> &update_interval_millisecs, std::atomic<bool>& vector_changed );
bool is_app_running = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createAction();
    createTrayIcon();

    connect(myTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayLeftClicked(QSystemTrayIcon::ActivationReason)));

    check_filter();

    std::wifstream iff; iff.open("userdata_ake.json");
    std::wstring qq, app_state;
    iff >> qq;
    std::wstringstream ss(qq);
    double val; ss >> val;
    ui->IntervalCheck->setValue(val);
    iff >> app_state;

    while(iff >> qq){
        app_list.emplace_back(qq);
        ui -> listCurrentApps -> addItem(QString::fromStdWString(qq));
    }
    iff.close();
    std::thread my_thread(run_autokillexplorer, std::ref(program_is_running), std::ref(update_interval_millisecs), std::ref(vector_changed));

    my_thread.detach();
    if(app_state == L"start"){
        is_app_running = true;
        program_is_running.store(true);
        ui->StartButton->setText("Stop");
        ui->AddButton->setEnabled(false);
        ui->RemoveButton->setEnabled(false);
        ui->listCurrentApps->setEnabled(false);
        ui->AddTextEdit->setEnabled(false);
        ui->IntervalCheck->setEnabled(false);
        unsigned int timemill = int(ui->IntervalCheck->value() * 1000);
        update_interval_millisecs.store(timemill);
        vector_changed.store(true);
        hide();
    }
    else{
        show();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Tray
void MainWindow::createAction()
{
    //свернуть
    minimizeAction = new QAction(tr("Minimize"), this);
    connect(minimizeAction,SIGNAL(triggered()), this, SLOT(hide()));

    //развернуть
    restoreAction = new QAction(tr("Open"), this);
    connect(restoreAction,SIGNAL(triggered()), this, SLOT(show()));

    //выход
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction(restoreAction);
    trayIconMenu -> addAction(minimizeAction);
    trayIconMenu -> addSeparator();
    trayIconMenu -> addAction(quitAction);

    myTray = new QSystemTrayIcon;
    myTray -> setIcon(QIcon(":/autokillexplorer_logo.ico"));
    myTray -> setVisible(true);
    myTray -> setContextMenu(trayIconMenu);
}

void MainWindow::onSystemTrayLeftClicked(QSystemTrayIcon::ActivationReason Reason)
{
    if (Reason == QSystemTrayIcon::DoubleClick)
    {
        if(isHidden())
            show();
        else
            hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (is_app_running)
    {
        if (!event->spontaneous() || !isVisible())
            return;
        if (myTray->isVisible()) {
            QMessageBox::information(this, tr("Systray"),
                                     tr("The program will keep running in the "
                                        "system tray. To terminate the program, "
                                        "choose <b>Quit</b> in the context menu "
                                        "of the system tray entry."));
            hide();
            event->ignore();
        }
    }
}


//AddButton
void MainWindow::on_AddButton_clicked()
{
    QString qqstring = ui->AddTextEdit->text().trimmed();
    std::wstring strwww = qqstring.toStdWString();
    transform(strwww.begin(), strwww.end(),strwww.begin(),::tolower);

    if (qqstring == ""){
        QMessageBox::critical(this, "Error", "Add application process name");
    }
    else if(strwww == L"explorer.exe"){
        QMessageBox::information(this, "Information", "You want to terminate <b>explorer.exe</b> when the <b>explorer.exe</b> starts, and start the <b>explorer.exe</b> when the <b>explorer.exe</b> is terminated?... \nSeriously?... ");
    }
    else{
        std::wstring newprocess = qqstring.toStdWString();

        std::wstring tmp;
        for(int i = int(newprocess.size()) - 1; i > int(newprocess.size()) - 5 and i > 0; --i){
            tmp += newprocess[i];
        }
        bool is_not_allowed_symbols = false;
        for(int i = 0; i < int(newprocess.size()); ++i){
            if (newprocess[i] == ' ' || newprocess[i] == '\n'){
                is_not_allowed_symbols = true;
                break;
            }
        }
        if(tmp == L"exe." && !is_not_allowed_symbols){
            bool is_uniq = true;

            for (int i = 0; i < ui -> listCurrentApps -> count(); ++i)
            {
                if (ui -> listCurrentApps -> item(i) ->text() == qqstring){
                    is_uniq = false;
                    QMessageBox::information(this, "Information", "This app is already on the list");
                    break;
                }
            }
            if (is_uniq){
                ui->listCurrentApps->addItem(qqstring);
                add_app_list(newprocess);
                vector_changed.store(true);
                std::wofstream outfile;
                outfile.open("userdata_ake.json", std::ios_base::app); // append instead of overwrite
                outfile << L'\n' << newprocess;
                outfile.close();
                ui->AddTextEdit->setText("");

            }
        }
        else{
            QMessageBox::information(this, "Information", "Please enter the name of the process according to the form <b>*.exe</b>, without spaces, tabs, etc.");
        }


    }
}

//RemoveButton
void MainWindow::on_RemoveButton_clicked()
{
    QString delstr = ui->listCurrentApps->currentItem()->text();
    std::wifstream in("userdata_ake.json");
    std::wofstream out("outfile.json");
    std::wstring line;

    getline(in, line);
    if(line != delstr.toStdWString()){
        out << line;
    }

    while (getline(in, line))
    {
        if(line != delstr.toStdWString()){
            out << '\n' << line;
        }
    }
    in.close();
    out.close();
    remove("userdata_ake.json");
    rename("outfile.json", "userdata_ake.json");

    int attr = GetFileAttributes(L"userdata_ake.json");
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(L"userdata_ake.json", attr | FILE_ATTRIBUTE_HIDDEN);
    }

    //Удаляем элемент из вектора
    del_app_list(delstr.toStdWString());
    vector_changed.store(true);
    ui->listCurrentApps->takeItem(ui->listCurrentApps->row(ui->listCurrentApps->currentItem()));
}

//StartButton
void MainWindow::on_StartButton_clicked()
{
    if(is_app_running){
        ui->StartButton->setText("Start");
        //myTray->showMessage("Programm stopped", "Now you can edit config or leave...", QSystemTrayIcon::MessageIcon(1), 1000);
        is_app_running = false;
        ui->AddButton->setEnabled(true);
        ui->RemoveButton->setEnabled(true);
        ui->listCurrentApps->setEnabled(true);
        ui->AddTextEdit->setEnabled(true);
        ui->IntervalCheck->setEnabled(true);
        program_is_running.store(false);

        std::wifstream in("userdata_ake.json");
        std::wofstream out("outfile.json");
        std::wstring line;
        getline(in, line);
        out << line;
        getline(in, line);
        out << L"\nstop";
        while (getline(in, line))
        {
            out << '\n' << line;

        }
        in.close();
        out.close();
        remove("userdata_ake.json");
        rename("outfile.json", "userdata_ake.json");

        int attr = GetFileAttributes(L"userdata_ake.json");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes(L"userdata_ake.json", attr | FILE_ATTRIBUTE_HIDDEN);
        }

        if(!is_process_running(L"explorer.exe")){
            QProcess *console=new QProcess();
            console->startDetached("C:\\Windows\\System32\\cmd.exe",QStringList() << "/K" << "explorer.exe");
            console->waitForReadyRead();
        }
        //
    }
    else{
        ui->StartButton->setText("Stop");
        //myTray->showMessage("Programm started", "Waiting for the app process...", QSystemTrayIcon::MessageIcon(1), 1000);
        is_app_running = true;
        ui->AddButton->setEnabled(false);
        ui->RemoveButton->setEnabled(false);
        ui->listCurrentApps->setEnabled(false);
        ui->AddTextEdit->setEnabled(false);
        ui->IntervalCheck->setEnabled(false);
        unsigned int timemill = int(ui->IntervalCheck->value() * 1000);
        update_interval_millisecs.store(timemill);
        vector_changed.store(true);

        std::wifstream in("userdata_ake.json");
        std::wofstream out("outfile.json");
        std::wstring line;

        getline(in, line);
        out << std::to_wstring(ui->IntervalCheck->value());
        getline(in, line);
        out << L"\nstart";
        while (getline(in, line))
        {
                out << '\n' << line;

        }
        in.close();
        out.close();
        remove("userdata_ake.json");
        rename("outfile.json", "userdata_ake.json");

        int attr = GetFileAttributes(L"userdata_ake.json");
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
                SetFileAttributes(L"userdata_ake.json", attr | FILE_ATTRIBUTE_HIDDEN);
        }

        program_is_running.store(true);
    }
}


std::wstring explorer = L"explorer.exe";
void run_autokillexplorer( std::atomic<bool>& program_is_running, std::atomic<unsigned int> &update_interval_millisecs, std::atomic<bool>& vector_changed )
{

    auto wait_duration = std::chrono::milliseconds(update_interval_millisecs) ;
    std::vector<std::wstring> apps;
    while( true )
    {
        if(vector_changed){
            apps = read_app_list();
            wait_duration = std::chrono::milliseconds(update_interval_millisecs);
            vector_changed.store(false);
        }

        if (program_is_running){
            if (is_process_running(explorer)) {
                for (auto &el : apps) {
                    if (is_process_running(el)) {
                        QProcess *console=new QProcess();
                        console->startDetached("C:\\Windows\\System32\\cmd.exe",QStringList() << "/K" << "taskkill /im explorer.exe /f");
                        console->waitForReadyRead();
                       // system("taskkill /im explorer.exe /f");
                        break;
                    }
                }
            }
            else {
                bool flag = true;
                for (auto &el : apps) {
                    if (is_process_running(el)) {
                        flag = false;
                        break;
                    }
                }
                if (flag){
                    QProcess *console=new QProcess();
                    console->startDetached("C:\\Windows\\System32\\cmd.exe",QStringList() << "/K" << "explorer.exe");
                    console->waitForReadyRead();
                    //system("explorer.exe");
                }
            }
        }
        std::this_thread::sleep_for(wait_duration) ;
    }

}
