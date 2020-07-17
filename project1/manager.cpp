#include "manager.h"
#include "ui_manager.h"
#include"land.h"
#include"managerstaff.h"
#include"managergoods.h"

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
}

Manager::~Manager()
{
    delete ui;
}


/**
 * @brief  进入到相应的界面
 * @param
 * @return
**/
void Manager::on_returnBtn_clicked()
{
    Land * l = new Land();
    l->show();
    this->close();
}

void Manager::on_staffManagerBtn_clicked()
{
    ManagerStaff * ms = new ManagerStaff();
    ms->show();
    this->close();
}

void Manager::on_goodsManagerBtn_clicked()
{
    ManagerGoods * mg = new ManagerGoods () ;
    mg->show();
    this->close();
}
