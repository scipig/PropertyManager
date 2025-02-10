#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->employeebutton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_pushButton_clicked()
{
    QString phone=ui->Username->text();
    QString password=ui->password->text();
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
    int T1=0;
    if(ui->managerbutton->isChecked()){//管理员登录
        QSqlQuery query(db);
        query.prepare("select phone_num,password,department,name,count from employee where phone_num=:phone");//查看手机号、密码、部门、姓名
        query.bindValue(":phone",phone);
        query.exec();
        while(query.next()){
            //query的内部位置最初位于结果集的 “前置位置”，也就是在第一条记录之前，next()函数的主要功能是将query的内部位置移动到结果集的下一条记录，并返回一个布尔值。
            QString phone1=query.value(0).toString();
            QString password1=query.value(1).toString();
            QString department1=query.value(2).toString();
            QString name1=query.value(3).toString();
            QString count1=query.value(4).toString();
            if(phone==phone1&&password==password1){//确认姓名和密码正确
                T1++;
                QSqlQuery managerquery(db);
                managerquery.prepare("select name from manager where department=:department");//查看部门经理
                managerquery.bindValue(":department",department1);
                managerquery.exec();
                while(managerquery.next()){
                    QString name2=managerquery.value(0).toString();
                    if(name2==name1){//确认拥有管理权限
                        this->career="manager";
                        T1++;
                        this->hide();
                        m=new managerWindow;
                        m->phone=phone;
                        m->name=name1;
                        m->department=department1;
                        m->count=count1;
                        m->career=this->career;
                        m->show();
                        m->showemployee();
                        m->showfee();
                    }
                }
                if(T1==1){
                    QMessageBox::information(this,tr("警告"),tr("该用户没有管理权限"));
                }

            }
        }
    }
    else{//普通员工登录
        QSqlQuery query(db);
        query.prepare("select phone_num,password,department,name,count from employee where phone_num=:phone");//查看手机号、密码、部门、姓名
        query.bindValue(":phone",phone);
        query.exec();
        while(query.next()){
            QString phone1=query.value(0).toString();
            QString password1=query.value(1).toString();
            QString department1=query.value(2).toString();
            QString name1=query.value(3).toString();
            QString count1=query.value(4).toString();
            if(phone==phone1&&password==password1){
                T1++;
                this->career="common";
                if(department1!="行政处"){//非行政员工只能打卡签到/修改密码
                    this->hide();
                    c=new clockin;
                    c->phone=phone;
                    c->name=name1;
                    c->department=department1;
                    c->count=count1;
                    c->career=this->career;
                    c->show();
                    c->showedit();
                }else{//行政员工能处理和查看业主的事物
                    this->hide();
                    e=new employee;
                    e->phone=phone;
                    e->name=name1;
                    e->department=department1;
                    e->count=count1;
                    e->career=this->career;
                    e->show();
                    e->showowner();
                }
            }
        }
    }
    if(T1==0){
        QMessageBox::information(this,tr("警告"),tr("用户名或密码错误!"));
    }
}

//管理员登录和普通用户登录只能二选一
void MainWindow::on_managerbutton_clicked()
{
    if(ui->employeebutton->isChecked()){
        ui->employeebutton->setChecked(false);
    }
}

void MainWindow::on_employeebutton_clicked()
{
    if(ui->managerbutton->isChecked()){
        ui->managerbutton->setChecked(false);
    }
}

