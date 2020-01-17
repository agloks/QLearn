#include <QMenu>
#include <QApplication>
#include <QMenuBar>
#include <QTextEdit>
#include <QStatusBar>
#include <QDebug>
#include <QListWidgetItem>
#include <QSound>

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
    TrayIcon* tray = new TrayIcon(this);

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

    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    connect(tray, &QSystemTrayIcon::activated, tray, &TrayIcon::showThis);
    connect(tray -> newAction, &QAction::triggered, tray, &TrayIcon::hideThis);
    connect(ui -> pushButton, &QPushButton::clicked, this, &MainWindow::on_saveButton_itemPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
{
    ui->timeEdit->selectAll();
    QSound::play(":/bomb-action.wav");
    this->checkTime();
}

void MainWindow::on_saveButton_itemPressed()
{
    const QString text(ui -> dateEdit -> text() + ' ' + ui -> timeEdit -> text() + ' ' + ui -> lineEdit -> text());
    db -> addDate(text);
    ui -> listWidget -> clear();
    ui -> listWidget -> addItems(db -> showAllDate());
}

void MainWindow::checkTime()
{
    QStringList allTimes = db->showAllDate();
    //standard sort is enough to organize it as we wanna
    allTimes.sort();

    QString dateNow = QDate::currentDate().toString();
    QString timeNow = QTime::currentTime().toString();
    qDebug() << dateNow + ' ' + timeNow;
}
