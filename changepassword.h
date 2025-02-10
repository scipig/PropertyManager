#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>
#include <QString>

namespace Ui {
class changepassword;
}

class changepassword : public QWidget
{
    Q_OBJECT

public:
    explicit changepassword(QWidget *parent = nullptr);
    ~changepassword();

    QString phone;

signals:
    void backclockin();

private slots:
    void on_back_clicked();

    void on_pushButton_clicked();

private:
    Ui::changepassword *ui;
};

#endif // CHANGEPASSWORD_H
