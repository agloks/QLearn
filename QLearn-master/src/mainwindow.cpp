#include <QMenu>
#include <QApplication>
#include <QMenuBar>
#include <QTextEdit>
#include <QStatusBar>
#include <QDebug>
#include <QListWidgetItem>
#include <QTimer>

#include "header/mainwindow.h"
#include "ui_mainwindow.h"
#include "header/trayicon.h"
#include "header/db.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/stream.png"));
    tray = new TrayIcon(this);

    QAction *quit = new QAction("&Quit", this);
    quit -> setShortcut(tr("CTRL+Q"));

    viewst = new QAction("&View statusbar", this);
    viewst -> setCheckable(true);
    viewst -> setChecked(false);

    QMenu *file;
    file = menuBar() -> addMenu("&file");
    file -> addAction(quit);
    file -> addSeparator();
    file -> addAction(tray -> newAction);
    file -> addSeparator();
    file -> addAction(viewst);

    db = new DbManager("./kkk");
    ui->listWidget->addItems(db -> showAllDate());

    qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::checkTime));
    timeSet = 2000;
    timer -> start(timeSet);

    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    connect(tray, &QSystemTrayIcon::activated, tray, &TrayIcon::showThis);
    connect(tray -> newAction, &QAction::triggered, tray, &TrayIcon::hideThis);
    connect(ui -> pushButton, &QPushButton::clicked, this, &MainWindow::on_saveButton_itemPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tray;
    delete viewst;
    delete db;
    delete timer;
}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
{
    ui->timeEdit->selectAll();
    this->checkTime();
}

void MainWindow::on_saveButton_itemPressed()
{
    const QString text(ui -> dateEdit -> text() + ' ' + ui -> timeEdit -> text() + ' ' + ui -> lineEdit -> text());
    db -> addDate(text);
    ui -> listWidget -> clear();
    ui -> listWidget -> addItems(db -> showAllDate());

    timeSet = 5000;
    timer -> start(timeSet);
}

bool MainWindow::assertTime(QString& timeSource, QString& timeDestin)
{
    /*
        (source | destin)[0] = hours
        (source | destin)[1] = minutes
    */
    const QStringList source = timeSource.split(":");
    const QStringList destin = timeDestin.split(":");

    if(source[0] == destin[0])
    {
        if(source[1] == destin[1])
        {
            return true;
        }
        else
        {
            int ms = source[1].toInt();
            int md = destin[1].toInt();
            int minutesRest = (ms > md) ? ms - md : md - ms;

            qDebug() << "2 else time; s = " << source[1] << "and dd = " << destin[1];
            timeSet = 1000 * 60 * minutesRest;
        }
    }
    else
    {
        int hs = source[0].toInt();
        int hd = destin[0].toInt();
        int hoursRest = (hs > hd) ? hs - hd : hd - hs;

        qDebug() << "1 else time; s = " << source[0] << "and dd = " << destin[0];
        timeSet = 1000 * 60  * 60 * hoursRest;
    }

    return false;
}

bool MainWindow::assertDate(QString& dateSource, QStringList& dateDestin)
{
    qDebug() << dateSource;
    /*
        dateSource = "2000/01/14 00:00" -> ("2000", "01", "14", "00:00", "")  dateDestin = ("qui", "jan", "23", "2020", "10:57:40")

        dateSource[1] = month  dateDestin[1] = month
        dateSource[0] = year    dateDestin[2] = day
        dateSource[2] = day   dateDestin[3] = year
        dateSource[3] = time   dateDestin[4] = time
    */
    QStringList source = dateSource.split(QRegExp("/|\\s"));
    QMap<QString, QString> months;
    months["jan"] = "01";
    months["feb"] = "02";
    months["mar"] = "03";
    months["apr"] = "04";
    months["may"] = "05";
    months["jun"] = "06";
    months["jul"] = "07";
    months["aug"] = "08";
    months["sep"] = "09";
    months["oct"] = "10";
    months["nov"] = "11";
    months["dec"] = "12";

    if(source[0] == dateDestin[3])
    {
        if(source[1] == months[dateDestin[1]])
        {
            if(source[2] == dateDestin[2])
            {
                qDebug() << "3 else date; s = " << source[0] << "and dd = " << dateDestin[2];
                return this -> assertTime(source[3], dateDestin[4]);
            }
        }
        else
        {
            qDebug() << "2 else date; s = " << source[1] << "and dd = " << months[dateDestin[1]];

            timeSet = 1000 * 60 * 60 * 24; //one day
        }
    }
    else
    {
        qDebug() << "1 else date; s = " << source[2] << "and dd = " << dateDestin[3];
        timeSet = 1000 * 60 * 60 * 24; //one day
    }

    return false;
}

void MainWindow::checkTime()
{
    QStringList allTimes = db->showAllDate();
    this->orderFull(allTimes);
    this -> setListToday(allTimes);

    if((*listToday).size())
    {
        QString nextTask = (*listToday)[0];
        const QString dateNow = QDate::currentDate().toString().toLower();
        const QString timeNow = QTime::currentTime().toString().toLower();
        QStringList now = (dateNow + ' ' + timeNow).split(" ");

        if(this -> assertDate(nextTask, now))
        {
            QString msg("lupanalatica");
            this -> tray -> showBox(msg, timeSet);
        }
        qDebug() << timeSet;
        timer -> start(timeSet);
    }
}
