#include "employee.h"
#include "ui_employee.h"
#include <QVariant>
#include <QMessageBox>

employee::employee(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::employee)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(2, 200);
}

employee::~employee()
{
    delete ui;
}

void employee::showowner(){
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
    if (!query.exec("SELECT name, phone_num, CONCAT(zone, '区', building, '栋', apartment_num, '单元', house_num) AS full_address, area, has_paid, management_fee FROM owner")) {
        QMessageBox::warning(this, tr("SQL Error"), tr("查询出错！"));
        return;
    }

    QSqlQuery queryfee(db);
    if(!queryfee.exec("SELECT feex from fee")){
        QMessageBox::warning(this, tr("SQL Error"), tr("查询出错！"));
    }
    float fee;
    if(queryfee.next()){
        fee=queryfee.value(0).toFloat();
    }
    int row=0;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);  // 清空所有行

    while(query.next()){
        // 添加新的一行
        ui->tableWidget->insertRow(row);

        // 获取每一列的数据
        QString name = query.value(0).toString();
        QString phone_num = query.value(1).toString();
        QString full_address = query.value(2).toString();  // 拼接后的地址
        QString area = query.value(3).toString();
        bool has_paid = query.value(4).toBool();
        float management_fee;

        QVariant mf = query.value("management_fee");
        if(mf.isNull()){
            management_fee=area.toFloat()*fee;
            //将计算的物业费写入数据库
            QSqlQuery cf(db);
            cf.prepare("UPDATE owner SET management_fee=:mf WHERE phone_num=:phone");
            cf.bindValue(":mf",management_fee);
            cf.bindValue(":phone",phone_num);
            cf.exec();
        }else{
            management_fee=query.value(5).toFloat();
        }

        // 将每个字段的数据设置到表格的相应单元格中
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(name));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(phone_num));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(full_address));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(area));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(management_fee)));//只能加载字符串

        // 将 "是否交了物业费" 字段设置为 "是" 或 "否"
        QString paidStatus = has_paid ? "是" : "否";
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(paidStatus));

        row++;  // 跳到下一行
    }

}



void employee::on_go_to_clockin_clicked()
{
    this->hide();
    ck=new clockin;
    ck->name=this->name;
    ck->phone=this->phone;
    ck->department=this->department;
    ck->count=this->count;
    ck->career=this->career;
    ck->show();
    ck->showedit();
    connect(ck,&clockin::goemployee,this,employee::show);
}


void employee::on_select_clicked()
{
    QString owner=ui->owner->text();
    QString phonex=ui->phone->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);  // 清空所有行
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
    query.prepare("select name, phone_num, CONCAT(zone, '区', building, '栋', apartment_num, '单元', house_num) AS full_address, area, has_paid, management_fee FROM owner where phone_num=:phone");
    query.bindValue(":phone",phonex);
    if(!query.exec()){
        QMessageBox::warning(this, tr("SQL Error"), tr("查询出错！"));
        return;
    }
    QSqlQuery queryfee(db);
    if(!queryfee.exec("SELECT feex from fee")){
        QMessageBox::warning(this, tr("SQL Error"), tr("查询出错！"));
    }
    float fee;
    if(queryfee.next()){
        fee=queryfee.value(0).toFloat();
    }
    int row=0;
    while(query.next()){
        // 添加新的一行
        ui->tableWidget->insertRow(row);

        // 获取每一列的数据
        QString name = query.value(0).toString();
        QString phone_num = query.value(1).toString();
        QString full_address = query.value(2).toString();  // 拼接后的地址
        QString area = query.value(3).toString();
        bool has_paid = query.value(4).toBool();
        float management_fee;

        QVariant mf = query.value("management_fee");
        if(mf.isNull()){
            management_fee=area.toFloat()*fee;
            //将计算的物业费写入数据库
            QSqlQuery cf(db);
            cf.prepare("UPDATE owner SET management_fee=:mf WHERE phone_num=:phone");
            cf.bindValue(":mf",management_fee);
            cf.bindValue(":phone",phone_num);
            cf.exec();
        }else{
            management_fee=query.value(5).toFloat();
        }

        // 将每个字段的数据设置到表格的相应单元格中
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(name));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(phone_num));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(full_address));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(area));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(management_fee)));//只能加载字符串

        // 将 "是否交了物业费" 字段设置为 "是" 或 "否"
        QString paidStatus = has_paid ? "是" : "否";
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(paidStatus));

        row++;  // 跳到下一行
    }
}


void employee::on_showall_clicked()
{
    this->showowner();
}


void employee::on_selectwidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void employee::on_modifywidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void employee::on_deletewidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void employee::on_addwidget_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}


void employee::on_modify_clicked()
{
    QString owner=ui->owner_2->text();
    QString phonex=ui->phone_2->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);  // 清空所有行
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
    query.prepare("UPDATE owner SET has_paid=true where phone_num=:phone");
    query.bindValue(":phone",phonex);
    if(query.exec()){
        QMessageBox::information(this,tr("成功"),tr("修改成功"));
    }
    this->showowner();
}


void employee::on_pushButton_clicked()
{
    QString owner=ui->owner_3->text();
    QString phonex=ui->phone_3->text();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);  // 清空所有行
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
    query.prepare("DELETE FROM owner where phone_num=:phone");
    query.bindValue(":phone",phonex);
    if(query.exec()){
        QMessageBox::information(this,tr("成功"),tr("删除成功"));
    }
    this->showowner();
}


void employee::on_add_clicked()
{
    QString ownername=ui->owner_4->text();
    QString phonex=ui->phone_4->text();
    QString zone=ui->zoneedit->text();
    QString building=ui->buildingedit->text();
    QString apartment=ui->apartmentedit->text();
    QString house_num=ui->houseedit->text();
    float area=ui->areaedit->text().toFloat();

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
            QMessageBox::warning(this,tr("失败"),tr("数据库加载失败！"));
            return;
        }
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO owner VALUES (:name,:phone,:zone,:building,:apartment,:house_num,:area,null,false)");
    query.bindValue(":name",ownername);
    query.bindValue(":phone",phonex);
    query.bindValue(":zone",zone);
    query.bindValue(":building",building);
    query.bindValue(":apartment",apartment);
    query.bindValue(":house_num",house_num);
    query.bindValue(":area",area);
    if(query.exec()){
        QMessageBox::information(this,tr("成功"),tr("添加成功"));
    }
    this->showowner();
}


void employee::on_go_to_managerwindow_clicked()
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
    query.prepare("SELECT name from manager where department=:department");
    query.bindValue(":department",this->department);
    query.exec();
    bool cg=false;
    while(query.next()){
        QString namex=query.value(0).toString();
        if(this->name==namex){
            cg=true;
        }
    }
    if(cg==true){
        emit gomanager();
        this->close();
    }else{
        QMessageBox::warning(this,tr("失败"),tr("您没有此权限！"));
    }
}

