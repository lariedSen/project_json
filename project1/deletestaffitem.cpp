#include "deletestaffitem.h"
#include "ui_deletestaffitem.h"
#include<QtDebug>
DeleteStaffitem::DeleteStaffitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeleteStaffitem)
{
    ui->setupUi(this);
    this->resize(350,50);
}

DeleteStaffitem::~DeleteStaffitem()
{
    delete ui;
}

void DeleteStaffitem::set(classData &cd)
{
    ui->num_label->setAlignment(Qt::AlignHCenter);
    ui->account_label->setAlignment(Qt::AlignHCenter);
    ui->passLabel->setAlignment(Qt::AlignHCenter);

    if(cd.ok)
    {
        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        ui->num_label->setText("序号");
        ui->account_label->setText("账号");
        ui->passLabel->setText("密码");

    }
    else{
        qDebug()<<"number:"<<cd.num;
        ui->num_label->setText(QString::number(cd.num));
        ui->account_label->setText(cd.account);
        ui->passLabel->setText(cd.password);
    }
    qDebug()<<"====================";qDebug();
}
