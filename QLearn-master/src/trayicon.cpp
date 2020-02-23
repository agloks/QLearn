#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>
#include <QSound>
#include <QMessageBox>

#include "header/mainwindow.h"
#include "header/trayicon.h"
#include "ui_mainwindow.h"

TrayIcon::TrayIcon(QWidget *parent)
    :QSystemTrayIcon(parent)
    , ui(new Ui::MainWindow)
{
    trayIcon = new QSystemTrayIcon(parent);
    trayIcon -> setIcon(QIcon(":/images/stream.png"));
    trayIcon -> setVisible(1);
    trayIcon -> setToolTip("running...");
    //    trayIcon -> showMessage("a cada tempo", "eu digo isso", QIcon(":/stream.png"),5000);

    MainWindow = parent;
    newAction = new QAction("&Hide", this);
    newAction -> setShortcut(tr("CTRL+W"));

    connect(newAction, &QAction::triggered, this, &TrayIcon::hideThis);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::showThis);
    connect(newAction, &QAction::triggered, this, &TrayIcon::hideThis);
}


void TrayIcon::showThis(unsigned int reason)
{
    qDebug() << reason;
    if(reason == 3) { MainWindow -> show(); }
    else { MainWindow -> hide(); }
}

void TrayIcon::hideThis()
{
    MainWindow -> hide();
}

void TrayIcon::showBox(QStringRef &text, int& t)
{
    QSound::play(":/bomb-action.wav");
    t = 1000 * 60; //one minute
    QMessageBox::information(nullptr , tr("Task"), QString(text.toLocal8Bit()));
}

TrayIcon::~TrayIcon()
{
    delete trayIcon;
}
