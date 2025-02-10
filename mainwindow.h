#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//登录界面
#include <QMainWindow>
#include "managerwindow.h"
#include "employee.h"
#include "clockin.h"
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString career;

private slots:


    void on_pushButton_clicked();

    void on_managerbutton_clicked();

    void on_employeebutton_clicked();

private:
    Ui::MainWindow *ui;
    managerWindow *m;
    employee *e;
    clockin *c;
};
#endif // MAINWINDOW_H
