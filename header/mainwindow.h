#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QListWidgetItem;
class DbManager;
class TrayIcon;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_itemPressed(QListWidgetItem *item);
    void on_saveButton_itemPressed();
    void checkTime();

private:
    Ui::MainWindow *ui;
    QAction *viewst;
    QString dateString;
    DbManager* db;
    TrayIcon* tray;
    QTimer *timer;
    int timeSet;

    friend class TrayIcon;
};
#endif // MAINWINDOW_H
