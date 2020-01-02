#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QSystemTrayIcon;
class SimpleMenu;
class QAction;
QT_END_NAMESPACE

class TrayIcon: public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(QWidget *parent);
    ~TrayIcon();

    QAction* newAction;

public slots:
    void showThis(unsigned int reason);
    void hideThis();

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon* trayIcon;
    QWidget* MainWindow;

};

#endif // TRAYICON_H
