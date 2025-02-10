#ifndef CLOCKIN_H
#define CLOCKIN_H
//打卡界面
#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include "changepassword.h"
namespace Ui {
class clockin;
}

class clockin : public QWidget
{
    Q_OBJECT

public:
    explicit clockin(QWidget *parent = nullptr);
    ~clockin();
    void showedit();

    QString phone;
    QString name;
    QString department;
    QString count;
    QString career;

private slots:

    void on_clockinbutton_clicked();

    void on_pushButton_clicked();

    void on_go_to_employee_clicked();

signals:
    void goemployee();

private:
    Ui::clockin *ui;
    QSqlDatabase db;
    changepassword *cp;
};

#endif // CLOCKIN_H
