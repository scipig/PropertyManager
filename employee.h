#ifndef EMPLOYEE_H
#define EMPLOYEE_H
//行政员工界面
#include <QWidget>
#include <QString>
#include "clockin.h"
namespace Ui {
class employee;
}

class employee : public QWidget
{
    Q_OBJECT

public:
    explicit employee(QWidget *parent = nullptr);
    ~employee();

    void showowner();

    QString phone;
    QString name;
    QString department;
    QString count;
    QString career;

    //float fee=0.5;//默认物业费为0.5元/平米

    clockin *ck;

signals:
    void gomanager();

private slots:

    void on_go_to_clockin_clicked();

    void on_select_clicked();

    void on_showall_clicked();

    void on_selectwidget_clicked();

    void on_modifywidget_clicked();

    void on_deletewidget_clicked();

    void on_addwidget_clicked();

    void on_modify_clicked();

    void on_pushButton_clicked();

    void on_add_clicked();

    void on_go_to_managerwindow_clicked();

private:
    Ui::employee *ui;
};

#endif // EMPLOYEE_H
