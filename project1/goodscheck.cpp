#include "goodscheck.h"
#include "ui_goodscheck.h"

GoodsCheck::GoodsCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoodsCheck)
{
    ui->setupUi(this);

    //设置数据靠左显示
    ui->label_type->setAlignment(Qt::AlignLeft);
    ui->label_man->setAlignment(Qt::AlignHCenter);
    ui->label_num->setAlignment(Qt::AlignLeft);
    ui->label_date->setAlignment(Qt::AlignLeft);
    ui->label_commod->setAlignment(Qt::AlignLeft);
    ui->label_unitPrice->setAlignment(Qt::AlignLeft);
    ui->label_InventorNumber->setAlignment(Qt::AlignLeft);

}

GoodsCheck::~GoodsCheck()
{
    delete ui;
}

/**
 * @brief  设置商品的
 * @param
 * @return
**/
void GoodsCheck::set(classData &cd)
{
    QString number;
    if(cd.ok)
    {
        ui->label_type->setText(cd.typeText);
        ui->label_man->setText(cd.name);
        ui->label_num->setText(number.setNum(cd.com_num,'f',2));
        ui->label_date->setText(cd.Date);
        ui->label_commod->setText(cd.commodity);
        ui->label_unitPrice->setText(number.setNum(cd.ShippingPrice,'f',2));
        ui->label_InventorNumber->setText(number.setNum(cd.PurchasePrice,'f',2));
    }
    else
    {
        ui->label_type->setText("采购类型");
        ui->label_man->setText("采购人");
        ui->label_num->setText("采购数量");
        ui->label_date->setText("采购日期");
        ui->label_commod->setText("商品");
        ui->label_unitPrice->setText("出货价格");
        ui->label_InventorNumber->setText("进货价格");
    }




}

/**
 * @brief  设置员工结算的
 * @param
 * @return
**/
void GoodsCheck::set_seettelAccout(classData &cd)
{

    QString number;
    if(cd.ok)
    {
        ui->label_type->setText(cd.typeText);
        ui->label_man->setText(cd.name);
        ui->label_num->setText(number.setNum(cd.com_num,'f',2));
        ui->label_date->setText(cd.Date);
        ui->label_commod->setText(cd.commodity);

        //改为金额显示
        ui->label_unitPrice->setText(number.setNum(cd.ShippingPrice,'f',2));
        ui->label_InventorNumber->setText(number.setNum(cd.PurchasePrice,'f',2));
    }
    else
    {
        ui->label_type->setText("采购类型");
        ui->label_man->setText("采购人");
        ui->label_num->setText("采购数量");
        ui->label_date->setText("采购日期");
        ui->label_commod->setText("商品");
        ui->label_unitPrice->setText("金额");
        ui->label_InventorNumber->setText("进货价格");
    }

}

/**
 * @brief  设置购物的显示
 * @param
 * @return
**/
void GoodsCheck::set_user_shopping(classData &cd)
{

    QString number;
    if(cd.ok)
    {
        ui->label_type->setText(cd.typeText);
        ui->label_man->setText(cd.name);//员工的名字
        ui->label_num->setText(number.setNum(cd.com_num,'f',2));
        ui->label_date->setText(cd.Date);
        ui->label_commod->setText(cd.commodity);

        //改为金额显示
        ui->label_unitPrice->setText(number.setNum(cd.ShippingPrice,'f',2));
        ui->label_InventorNumber->setText(number.setNum(cd.PurchasePrice,'f',2));
    }
    else
    {
        ui->label_type->setText("物品类型");
        ui->label_man->setText("收银员");
        ui->label_num->setText("数量");
        ui->label_date->setText("日期");
        ui->label_commod->setText("商品");
        ui->label_unitPrice->setText("金额");
        ui->label_InventorNumber->setText("单价");
    }

}
