#include "clockin.h"
#include "ui_clockin.h"
#include <QMessageBox>
#include <QDebug>
clockin::clockin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::clockin)
{
    ui->setupUi(this);
}

clockin::~clockin()
{
    delete ui;
}

void clockin::showedit(){
    ui->nameedit->setText(name);
    ui->phoneedit->setText(phone);
    ui->departmentedit->setText(department);
    ui->countedit->setText(count);

    QSqlDatabase db;
    if(QSqlDatabase::contains("propertymanagement")){
        db = QSqlDatabase::database("propertymanagement");
    } else {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("propertymanagement");
        db.setUserName("root");
        db.setPassword("123456");
        if (!db.open()) {
            QMessageBox::warning(this, tr("失败"), tr("数据加载失败！"));
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT count_last_changed_date FROM employee WHERE phone_num = :phone");
    query.bindValue(":phone", this->phone);
    query.exec();
    if (query.next()) {
        QVariant date = query.value("count_last_changed_date");
        if (!date.isNull()) {
            QString lastDate = date.toString();
            QSqlQuery tquery(db);
            tquery.exec("SELECT CURDATE()");
            if (tquery.next()) {
                QString currentDate = tquery.value(0).toString();
                if (lastDate == currentDate) {
                    // 如果日期相同，表示已打卡
                    ui->clockinbutton->setStyleSheet("background-color: rgb(85, 255, 127);");
                    ui->clockinbutton->setText("已打卡");
                    ui->clockinbutton->setStyleSheet("color: rgb(255, 71, 14);");
                }
            }
        }
    }

}




void clockin::on_clockinbutton_clicked()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("propertymanagement")){
        db=QSqlDatabase::database("propertymanagement");
    }else{
        db=QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("propertymanagement");
        db.setUserName("root");
        db.setPassword("123456");
        if(!db.open()){
            QMessageBox::warning(this,tr("失败"),tr("数据路加载失败！"));
            return;
        }
    }
    QSqlQuery query(db);
    query.prepare("SELECT count_last_changed_date from employee where phone_num=:phone");
    query.bindValue(":phone",this->phone);
    query.exec();
    while(query.next()){
        QVariant date = query.value("count_last_changed_date");
        if(date.isNull()){
            QSqlQuery acquery(db);
            //更新本月打卡次数
            acquery.prepare("UPDATE employee SET count=count+1 where phone_num=:phone");
            acquery.bindValue(":phone",this->phone);
            acquery.exec();

            //更新打卡日期
            acquery.prepare("UPDATE employee SET count_last_changed_date = CURDATE() where phone_num=:phone");
            acquery.bindValue(":phone",this->phone);
            acquery.exec();

            int count1=this->count.toInt();
            count1++;
            this->count=QString::number(count1);
            this->showedit();
            ui->clockinbutton->setStyleSheet("background-color: rgb(85, 255, 127);");
            ui->clockinbutton->setText("已打卡");
            ui->clockinbutton->setStyleSheet("color:rgb(255, 71, 14)");
        }else{
            // 如果日期不为空，检查日期是否已经是今天
            QSqlQuery tquery(db);
            tquery.exec("SELECT CURDATE()");
            if (tquery.next()) { // 确保获取当前日期
                QString last_date = query.value(0).toString();
                QString cur_date = tquery.value(0).toString();
                if (cur_date != last_date) {
                    // 如果日期不同，说明今天尚未打卡，执行打卡
                    QSqlQuery acquery(db);
                    acquery.prepare("UPDATE employee SET count = count + 1 WHERE phone_num = :phone");
                    acquery.bindValue(":phone", this->phone);
                    acquery.exec();

                    // 更新打卡日期
                    acquery.prepare("UPDATE employee SET count_last_changed_date = CURDATE() WHERE phone_num = :phone");
                    acquery.bindValue(":phone", this->phone);
                    acquery.exec();

                    int count1 = this->count.toInt();
                    count1++;
                    this->count = QString::number(count1);
                    this->showedit();

                    ui->clockinbutton->setStyleSheet("background-color: rgb(85, 255, 127);");
                    ui->clockinbutton->setText("已打卡");
                    ui->clockinbutton->setStyleSheet("color: rgb(255, 71, 14);");
                } else {
                    QMessageBox::information(this, tr("提醒"), tr("今日已打卡"));
                }
            } else {
                qDebug() << "获取当前日期失败";
            }
        }
    }

}

//修改密码
void clockin::on_pushButton_clicked()
{
    cp=new changepassword;
    cp->phone=this->phone;
    this->hide();
    cp->show();
    connect(cp,&changepassword::backclockin,this,clockin::show);
}


void clockin::on_go_to_employee_clicked()
{
    QDebug debug = qDebug();
    debug << career;
    if(department=="行政处"||this->career=="manager"){
        emit goemployee();
        this->close();
    }else{
        QMessageBox::information(this, tr("提醒"), tr("您没有此权限"));
    }
}

