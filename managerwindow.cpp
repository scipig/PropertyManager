#include "managerwindow.h"
#include "ui_managerwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
managerWindow::managerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::managerWindow)
{
    ui->setupUi(this);
}

managerWindow::~managerWindow()
{
    delete ui;
}

void managerWindow::on_go_employeewindow_clicked()
{
    this->hide();
    ee=new employee;
    ee->name=this->name;
    ee->phone=this->phone;
    ee->department=this->department;
    ee->count=this->count;
    ee->career=this->career;
    ee->show();
    connect(ee,&employee::gomanager,this,managerWindow::show);
}

void managerWindow::showemployee(){
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
    query.prepare("SELECT name,phone_num,count,count_last_changed_date FROM employee where department=:department");
    query.bindValue(":department",this->department);
    if(!query.exec()){
        QMessageBox::warning(this, tr("SQL Error"), tr("查询出错！"));
        return;
    }
    int row=0;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    while(query.next()){
        ui->tableWidget->insertRow(row);
        QString name=query.value(0).toString();
        QString phonex=query.value(1).toString();
        QString count=query.value(2).toString();
        QString date=query.value(3).toString();

        ui->tableWidget->setItem(row,0,new QTableWidgetItem(this->department));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(name));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(phonex));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(count));
        ui->tableWidget->setItem(row,4,new QTableWidgetItem(date));

        row++;
    }
}

void managerWindow::showfee(){
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
    query.exec("SELECT feex from fee");
    if(query.next()){
        ui->fee->setText(query.value(0).toString());
    }
}


void managerWindow::on_selectwidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void managerWindow::on_changewidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}



void managerWindow::on_deletewidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void managerWindow::on_addwidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}


void managerWindow::on_select_clicked()
{
    QString name1=ui->name->text();
    QString phone1=ui->phone->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
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
    query.prepare("SELECT name,phone_num,count,count_last_changed_date FROM employee where phone_num=:phone");
    query.bindValue(":phone",phone1);
    query.exec();
    if(query.next()){
        ui->tableWidget->insertRow(0);
        QString name=query.value(0).toString();
        QString phonex=query.value(1).toString();
        QString count=query.value(2).toString();
        QString date=query.value(3).toString();

        ui->tableWidget->setItem(0,0,new QTableWidgetItem(this->department));
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(name));
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(phonex));
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(count));
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(date));
    }
}


void managerWindow::on_change_clicked()
{
    QString name1=ui->name_2->text();
    QString phone1=ui->phone_2->text();
    QString department1=ui->departmentedit->text();
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
    if(!department1.isEmpty()){
        QSqlQuery query(db);
        query.prepare("UPADTE employee SET department=:department where phone_num=:phone");
        query.bindValue(":department",department1);
        query.bindValue(":phone",phone1);
        if(query.exec()){
            QMessageBox::information(this,tr("成功"),tr("修改成功"));
        }else{
            QMessageBox::warning(this,tr("失败"),tr("修改失败"));
        }

        if(ui->radioButton->isChecked()){
            QSqlQuery dquery(db);
            dquery.prepare("INSERT INTO manager VALUES (:department,:name)");
            dquery.bindValue(":department",department1);
            dquery.bindValue(":name",name1);
            dquery.exec();
        }
    }else{
        QSqlQuery query(db);
        query.prepare("SELECT department FROM employee WHERE phone_num=:phone");
        query.bindValue(":phone",phone1);
        query.exec();
        if(query.next()){
            QString departmentx=query.value(0).toString();
            if(ui->radioButton->isChecked()){
                QSqlQuery dquery(db);
                dquery.prepare("INSERT INTO manager VALUES (:department,:name)");
                dquery.bindValue(":department",departmentx);
                dquery.bindValue(":name",name1);
                dquery.exec();
            }
        }
    }
    this->showemployee();
}


void managerWindow::on_delete_2_clicked()
{
    QString name1=ui->name_3->text();
    QString phone1=ui->phone_3->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
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
    query.prepare("DELETE FROM employee where phone_num=:phone");
    query.bindValue(":phone",phone1);
    query.exec();
    this->showemployee();
}


void managerWindow::on_pushButton_clicked()
{
    QString name1=ui->name_4->text();
    QString phone1=ui->phone_4->text();
    QString departmentx=ui->department2->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
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
    query.prepare("INSERT INTO employee (name, phone_num, department, password, count, count_last_changed_date) "
                  "VALUES (:name, :phone, :department, :password, :status, :date_created)");

    query.bindValue(":name", name1);
    query.bindValue(":phone", phone1);
    query.bindValue(":department", departmentx);
    query.bindValue(":password", "123456");  // 绑定固定密码
    query.bindValue(":status", 0);  // 状态为0
    query.bindValue(":date_created", QVariant());  // 这里传 null 或者当前时间

    if(query.exec()) {
        QMessageBox::information(this, tr("成功"), tr("加入成功!"));
    } else {
        QMessageBox::warning(this, tr("失败"), tr("加入失败!"));
    }
    this->showemployee();
}


void managerWindow::on_showall_clicked()
{
    this->showemployee();
}


void managerWindow::on_change_fee_clicked()
{
    float feex=ui->fee->text().toFloat();
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
    query.prepare("UPDATE fee SET feex=:fee");
    query.bindValue(":fee",feex);
    query.exec();
    this->showfee();
}

