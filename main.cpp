#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    foreach (const QString &driver, QSqlDatabase::drivers()) {
        qDebug() << driver;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
