#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H
//经理（管理员界面）
#include <QWidget>
#include "managerwindow.h"
#include <QString>
#include "employee.h"
namespace Ui {
class managerWindow;
}

class managerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit managerWindow(QWidget *parent = nullptr);
    ~managerWindow();
    QString phone;
    QString name;
    QString department;
    QString count;
    QString career;

    void showemployee();
    void showfee();



private slots:
    void on_go_employeewindow_clicked();



    void on_selectwidget_clicked();

    void on_changewidget_clicked();

    void on_deletewidget_clicked();

    void on_addwidget_clicked();

    void on_select_clicked();

    void on_change_clicked();

    void on_delete_2_clicked();

    void on_pushButton_clicked();

    void on_showall_clicked();

    void on_change_fee_clicked();

private:
    Ui::managerWindow *ui;
    employee *ee;

};

#endif // MANAGERWINDOW_H
