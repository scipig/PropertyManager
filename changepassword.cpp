#include "changepassword.h"
#include "ui_changepassword.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
changepassword::changepassword(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::changepassword)
{
    ui->setupUi(this);
}

changepassword::~changepassword()
{
    delete ui;
}

void changepassword::on_back_clicked()
{
    emit backclockin();//发射信号
    this->close();
}


void changepassword::on_pushButton_clicked()
{
    QString oripassword=ui->originpassword->text();
    QString newpassword1=ui->newpassword1->text();
    QString newpassword2=ui->newpassword2->text();

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
    query.prepare("SELECT password FROM employee where phone_num=:phone");
    query.bindValue(":phone",this->phone);
    query.exec();
    if(query.next()){
        QString op=query.value(0).toString();
        if(op==oripassword){
            if(newpassword1==newpassword2){
                QSqlQuery changequery(db);
                changequery.prepare("UPDATE employee SET password=:newpassword WHERE phone_num=:phone");
                changequery.bindValue(":newpassword",newpassword1);
                changequery.bindValue(":phone",this->phone);
                if(changequery.exec()){
                    QMessageBox::information(this, tr("成功"), tr("密码修改成功"));
                }else{
                    QMessageBox::warning(this, tr("错误"), tr("密码修改失败: "));
                }
            }
            else{
                QMessageBox::warning(this,tr("错误"),tr("两遍新密码不同，请重新输入！"));
            }
        }else{
            QMessageBox::warning(this,tr("错误"),tr("原始密码输入错误！"));
        }
    }


}

