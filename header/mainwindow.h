#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QListWidgetItem;
class DbManager;
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

private:
    Ui::MainWindow *ui;
    QAction *viewst;
    QString dateString;
    DbManager* db;
    void checkTime();

    friend class TrayIcon;
};
#endif // MAINWINDOW_H
