#include "goods.h"
#include "ui_goods.h"

Goods::Goods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Goods)
{
    ui->setupUi(this);
    ui->goodsLabel->setAlignment(Qt::AlignHCenter);
    ui->label_num->setAlignment(Qt::AlignHCenter);
    ui->label_price->setAlignment(Qt::AlignHCenter);
}

Goods::~Goods()
{
    delete ui;
}

/**
 * @brief 设置用户购物
 * @param
 * @return
**/
void Goods::setUser(classData &cd)
{
    QString number ;
    if(cd.ok)
    {
        ui->goodsLabel->setText(cd.commodity);
        ui->label_num->setText(number.setNum(cd.com_num,'f',2));
        ui->label_price->setText(number.setNum(cd.price,'f',2));
    }
    else{
        ui->goodsLabel->setText("商品");
        ui->label_num->setText("数量");
        ui->label_price->setText("单价");
    }
}

/**
 * @brief  设置商品的选择
 * @param
 * @return
**/
void Goods::set(classData &cd)
{
    QString number ;
    if(cd.ok)
    {
        ui->goodsLabel->setText(cd.commodity);
        ui->label_num->setText(number.setNum(cd.com_num,'f',2));
        ui->label_price->setText(number.setNum(cd.price,'f',2));
    }
    else{
        ui->goodsLabel->setText("商品");
        ui->label_num->setText("库存数量");
        ui->label_price->setText("单价价格");
    }
}
