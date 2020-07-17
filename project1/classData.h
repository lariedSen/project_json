#ifndef CLASSDATA_H
#define CLASSDATA_H

#include <QWidget>
class classData : public QWidget
{
    Q_OBJECT
public:


    bool ok;
    QString account;
    QString password;


     int num;

    QString typeText;//商品类型
    QString commodity; //商品名字
    float com_num; //   库存量/输入数量
    float price ;  //   单件价格
    float PurchasePrice ; //进货价格
    float ShippingPrice ;//出货价格

    QString name ;//名字
    QString Date;//日期

};



#endif // CLASSDATA_H

