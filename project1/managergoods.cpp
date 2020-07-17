#include "managergoods.h"
#include "ui_managergoods.h"
#include<QtDebug>
#include"manager.h"
#include"goods.h"
#include<QMessageBox>
#include<QDateTime>
#include"goodscheck.h"
#include<QFileInfo>

/*************************************************
Copyright:
Author:Laried
Date to update:2020年7月16日
Description ： 管理员的货物管理功能
[1]	采购商品：
[2]查询采购清单
[3]查询商场商品的销售记录
**************************************************/
ManagerGoods::ManagerGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerGoods)
{
    ui->setupUi(this);

    //1. 设置路径
    pathCommod = "../project1/json/commodity.json";
    purcharseList = "../project1/json/PurchaseList.json";
    tempList = "../project1/json/tempPur.json";
    shopDoc = "../project1/json/ShopDoc.json";

    //2.清空临时文档
    QFile filenew(tempList);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {

    }
    filenew.close();

    //3. 正则表达式限制输入
    QRegExp regExp("[0-9.0-9]*");
    ui->lineEdit_num->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_PurchasePrice->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_ShippingPrice->setValidator(new QRegExpValidator(regExp, this));

    //3.选择商品类型实时显示视图
    connect(ui->purchasComboBox,&QComboBox::currentTextChanged,this,&ManagerGoods::comboxType);
    QString text = ui->purchasComboBox->currentText();
    connect(this,&ManagerGoods::comboxTextchange,this,&ManagerGoods::comboxType);
    emit this->comboxTextchange(text);

    //5.绑定返回
    connect(ui->reutnrn1,&QAbstractButton::clicked,this,&ManagerGoods::reutnrnslots);
    connect(ui->returnBtn2,&QAbstractButton::clicked,this,&ManagerGoods::reutnrnslots);

    //6.设置查询的订单号
    ui->comboBox->clear();
    QJsonObject objList =  read_obj(purcharseList);
    QJsonArray arrayList= objList.value("采购清单").toArray();
    for(int i =arrayList.size() ;i>0;i--)
    {
        ui->comboBox->addItem(QString::number(i));
    }

    //7. 设置最开始的订单显示和实时更新显示
    on_pushButton_check_clicked();
    connect(ui->comboBox,&QComboBox::currentTextChanged,
            this,&ManagerGoods::on_pushButton_check_clicked);

    ui->purchaseBtn_finish->setEnabled(false);

    //8.销售查询
    QStringList getList2 = getCombox_user();
    connect(ui->comboBox_cheack,&QComboBox::currentTextChanged,this,&ManagerGoods::showListwidget);
    connect(this,&ManagerGoods::UserListwidget,this,&ManagerGoods::showListwidget);
    if(!getList2.isEmpty())
    {
         emit this->UserListwidget(ui->comboBox_cheack->currentText());
    }


}

/**
 * @brief   ui->comboBox_cheack的设置
 *         用于购物订单号的查询
 * @param
 * @return  返回购物订单号的列表
**/
QStringList ManagerGoods::getCombox_user()
{
    QJsonObject big_Obj =  read_obj(shopDoc);
    QJsonArray big_array = big_Obj.value("购物清单").toArray();
    QStringList num_list;
    for(int i =0 ;i<big_array.size();i++)
    {
        QJsonObject read_obj = big_array.at(i).toObject();
        int  Order_num = read_obj.value("订单编号").toInt();
        num_list<<QString::number(Order_num);

    }
    ui->comboBox_cheack->addItems(num_list);
    return num_list;
}



ManagerGoods::~ManagerGoods()
{
    delete ui;
}



/**
 * @brief  获取里面的数据（最外面的大括号对象数据）
 *  Json数据文档通用
 * @param
 * @return QJsonObject对象
**/
QJsonObject ManagerGoods::read_obj(QString pathFile)
{
    QFile file(pathFile);
    file.open(QIODevice::ReadOnly);
    QByteArray array_read  = file.readAll();
    QJsonDocument doc =  QJsonDocument::fromJson(array_read);
    QJsonObject obj = doc.object();
     file.close();
     return obj;
}

/**
 * @brief  获取类型数组中的对应的commodity所在的json对象
 * @param
 * @return  这个接口好像没什么用
**/
QJsonObject ManagerGoods::getInfo(QJsonArray js_array, QString commodity)
{
    for(int i =0 ; i<js_array.size();i++)
    {
        QJsonObject obj  = js_array.at(i).toObject();
        if(commodity == obj.value("商品").toString())
        {
             return obj;
        }
    }
}

/**
 * @brief  把QJsonObject对象的数据写入到文件中
 * @param
 *          QJsonObject include_obj  --------该对象
 *          QString pathFile  ---------------文件路径
 * @return
**/
void  ManagerGoods::writeJsonFile(QJsonObject include_obj, QString pathFile)
{
    QFile filenew(pathFile);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
       QMessageBox::warning(this,tr("提示"),tr("清空失败"));
    }
    //写入文件
    QJsonDocument docDecu;
    docDecu.setObject(include_obj);
    QByteArray data = docDecu.toJson();
    qint64 if_d = filenew.write(data);
    filenew.close();
    if(if_d == -1)
    {
        QMessageBox::warning(this,tr("温馨提示"),tr("操作失败"),QMessageBox::Close);
    }
//    else {
//         QMessageBox::information(this,tr("温馨提示"),tr("操作完成"),QMessageBox::Yes);
    //    }
}



/**
 * @brief  采购按键的槽函数
 * @param
 * @return
**/
void ManagerGoods::on_purchaseBtn_clicked()
{

    //1.获取输入的内容
    QString typeText =   ui->purchasComboBox->currentText();
    QString purchasText = ui->comboBox_commodity->currentText();
    QString strnum =  ui->lineEdit_num->text();
    float getnum= strnum.toFloat();
    QString strpur = ui->lineEdit_PurchasePrice->text();
    float PurchasePrice =  strpur.toFloat();//进货
    QString strShip= ui->lineEdit_ShippingPrice->text();
    float ShippingPrice = strShip.toFloat();//出货
    QString name = ui->lineEdit_man->text();

    //1.1.条件设置不允许空输入
    if(strnum.isEmpty()||strpur.isEmpty()||strShip.isEmpty()||name.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请不要空白输入"),QMessageBox::Close);
        return ;
    }

    //1.2设置按键状态
    ui->purchaseBtn->setEnabled(true);
    ui->purchaseBtn_finish->setEnabled(true);
    ui->reutnrn1->setEnabled(false);
    ui->returnBtn2->setEnabled(false);

    //2.判断库存量是否充足
    //2.1 先获取商品所在的对象
    QJsonObject big_obj = read_obj(pathCommod);
    QJsonObject type_obj =  big_obj.value("type").toObject();
    QJsonArray type_array = type_obj.value(typeText).toArray();
    QJsonObject goods_obj = getInfo(type_array, purchasText);//获取到商品所在的对象
    //2.2 所在对象的库存量是否充足
    QString goods =  goods_obj.value("商品").toString();
    float num = goods_obj.value("库存数量").toDouble();
    float price =  goods_obj.value("单件价格").toDouble();
    qDebug()<<"====" << num << price ;
    if(num< getnum)
    {
        QMessageBox::warning(this,tr("温馨提示"),
                             tr("很抱歉！库余量不足\n 请重新输入要购买的数量"),QMessageBox::Close);
        ui->lineEdit_num->clear();
        return ;
    }

    //3. 更改所在对象的库存量
    num = num-getnum;

    //3.1 被改动的对象，更新到原数组
    QJsonObject newObj;
    newObj.insert("商品",goods);
    newObj.insert("库存数量",num);
    newObj.insert("单件价格",price);
    for(int i =0 ;i<type_array.size();i++)
    {
        QJsonObject obj  =type_array.at(i).toObject();
        if(purchasText == obj.value("商品").toString())
        {
             type_array.removeAt(i);
             type_array.insert(i,newObj);
             break;
        }
    }

    //3.2重新写入文件
    QStringList list = type_obj.keys();
    QJsonObject obj_new;
    for (int i =0 ;i<type_obj.size();i++)
    {
        QJsonArray array_type;
        if(list.at(i) == typeText)
            array_type = type_array;
        else
            array_type= type_obj.value(list.at(i)).toArray();

        obj_new.insert(list.at(i),array_type);
    }
    QJsonObject change_obj;
    change_obj.insert("type",obj_new);
    writeJsonFile(change_obj,pathCommod);

    //4. 写入采购清单
    QString Date = QDateTime::currentDateTime().date().toString("yyyy/MM/dd");
    //4.1要新写入的商品购买数据
    QJsonObject goods1 ;
    goods1.insert("采购数量",getnum);
    goods1.insert("商品",purchasText);
    goods1.insert("进货价格",PurchasePrice);
    goods1.insert("出货价格",ShippingPrice);
    goods1.insert("采购人",name);

    //开始采购，
    //每次都会删除临时清单
    //4.2 判断是否位空文件
        QFile file(tempList);
        file.open(QIODevice::ReadOnly);
        QJsonObject typeObj ;
        QJsonArray arrayType ;
        if(file.size() == 0)
        {
            file.close();
            //如果是新创建的空文件
            arrayType.prepend(goods1);
            typeObj.insert(typeText,arrayType);
        }
        else{
            file.close();
            QJsonObject big_obj = read_obj(tempList);
            typeObj =  big_obj.value("采购种类").toObject();
            arrayType =  typeObj.value(typeText).toArray();
            arrayType.prepend(goods1);
            typeObj.remove(typeText);
            typeObj.insert(typeText,arrayType);
        }


        //4.3 数据层次包装
        QJsonObject include_obj;
        //采购种类
        include_obj.insert("采购种类",typeObj);
        //4.4 写入到临时文件中
        writeJsonFile(include_obj,tempList);

        //实时显示库存量
        emit this->comboxTextchange(ui->purchasComboBox->currentText());

    //员工结算后把每次采购的数据写入商场商品.josn
    //      应用场景：采购的清单在员工付款后，就可以变成商场自己的东西了


       // ui->lineEdit_man->clear();
        ui->lineEdit_num->clear();
        ui->lineEdit_PurchasePrice->clear();
        ui->lineEdit_ShippingPrice->clear();
}

/**
 * @brief  采购完成： 写入到采购清单PurchaseList.json
 * @param
 * @return
**/
void ManagerGoods::on_purchaseBtn_finish_clicked()
{
    if(ui->lineEdit_num->text().isEmpty()
            &&ui->lineEdit_PurchasePrice->text().isEmpty()
            &&ui->lineEdit_ShippingPrice->text().isEmpty())
    {

    }
    else{
        return ;
    }

    //读取临时文件
    QJsonObject frist_obj = read_obj(tempList);//采购种类
    QJsonObject wantObj = frist_obj.value("采购种类").toObject();

    //追加进去
    QJsonObject big_obj = read_obj(purcharseList);
    QJsonArray middle_array = big_obj.value("采购清单").toArray();

    QJsonObject small_obj ;
     QString Date = QDateTime::currentDateTime().date().toString("yyyy/MM/dd");
    small_obj.insert("订单编号",middle_array.size()+1);
    small_obj.insert("采购日期",Date);
    small_obj.insert("是否付款" ,false);
    small_obj.insert("采购种类",wantObj);
    middle_array.prepend(small_obj);

    QJsonObject include_obj ;
    include_obj.insert("采购清单",middle_array);
    writeJsonFile(include_obj,purcharseList);

    //清空临时文档
    QFile filenew(tempList);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {

    }
    filenew.close();

    //设置查询编号
    ui->comboBox->clear();
    for(int i =middle_array.size() ;i>0;i--)
    {
        ui->comboBox->addItem(QString::number(i));
    }


    ui->purchaseBtn->setEnabled(true);
    ui->purchaseBtn_finish->setEnabled(false);
    ui->reutnrn1->setEnabled(true);
    ui->returnBtn2->setEnabled(true);

}



/**
 * @brief  销售查询
 * @param
 * @return
**/
void ManagerGoods::on_pushButton_check_clicked()
{

    ui->listWidget_cheack->clear();
    int comboxnum =   ui->comboBox->currentText().toInt();
    //0.设置列表头
    setHeadCheak();

    //1.获取需要查询的内容
    QJsonObject commodObj =  read_obj(purcharseList);
    QJsonArray  fristArray = commodObj.value("采购清单").toArray();
    QJsonObject senondObj;
    for(int i=0;i<fristArray.size();i++)
    {
        senondObj = fristArray.at(i).toObject();
        if(comboxnum == senondObj.value("订单编号").toInt())
        {
            break;
        }
    }

    //2. 获取查询订单的 内容 并显示
    QString date = senondObj.value("采购日期").toString();
    QJsonObject typeObj =senondObj.value("采购种类").toObject();
    QStringList list_type = typeObj.keys();
    for(int i=0;i<list_type.size();i++)
    {
        QString typeName = list_type.at(i);//商品种类
        QJsonArray type_array = typeObj.value(typeName).toArray();
        for(int j=0;j<type_array.size();j++)
        {
            //商品种类中的对象之一
            QJsonObject obj = type_array.at(j).toObject();
            float ShippingPrice =  obj.value("出货价格").toDouble();
            float PurchasePrice = obj.value("进货价格").toDouble();
            float num = obj.value("采购数量").toDouble();
            QString commodity =  obj.value("商品").toString();
            QString man = obj.value("采购人").toString();

            classData cd1;
            cd1.ok = true;
            cd1.ShippingPrice = ShippingPrice;
            cd1.PurchasePrice = PurchasePrice;
            cd1.com_num = num;
            cd1.Date = date;
            cd1.name = man;
            cd1.typeText = typeName;
            cd1.commodity = commodity;

            QListWidgetItem *item = new  QListWidgetItem ();
            GoodsCheck * gc = new GoodsCheck();
            gc->set(cd1);
            item->setSizeHint(QSize(650,47));
            ui->listWidget_cheack->addItem(item);
            ui->listWidget_cheack->setItemWidget(item,gc);

        }

    }

}


/**
 * @brief  查询listwidget 的表头
 * @param
 * @return
**/
void ManagerGoods::setHeadCheak()
{
    qDebug()<<"*_==========================================";
    //创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    GoodsCheck * gc = new GoodsCheck();

    //第二点：设置条目窗口内容
    classData cd ;
    cd.ok = false ;
    gc->set(cd);

    //
    ui->listWidget_cheack->addItem(item); // 添加条目
    item->setSizeHint(QSize(650,47)); //最好要设置窗口的大小一致
    ui->listWidget_cheack->setItemWidget(item,gc);//条目窗口显示

    qDebug()<<"*******************************";
}

/**
 * @brief  货物listwidget 的表头
 * @param
 * @return
**/
void ManagerGoods::setHead()
{
    //创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    Goods *goodWidget = new Goods();

    //第二点：设置条目窗口内容
    classData cd ;
    cd.ok = false ;
    goodWidget->set(cd);

    //
    ui->listWidget->addItem(item); // 添加条目
    item->setSizeHint(QSize(433,45)); //最好要设置窗口的大小一致
    ui->listWidget->setItemWidget(item,goodWidget);//条目窗口显示

}

/**
 * @brief 选择商品时，listwidget 也随之变化
 * @param
 *      QString text ---- 商品的名称
 * @return
**/
void ManagerGoods::comboxType(QString text)
{
    ui->listWidget->clear();
    ui->comboBox_commodity->clear();
    qDebug()<<text;
    //1. 读取商品的数据
    QJsonObject commodObj =  read_obj(pathCommod);
    QJsonObject typeObj = commodObj.value("type").toObject();

    QStringList commodityList ;
    QJsonArray textArray = typeObj.value(text).toArray();

    setHead();
    for(int i=0;i<textArray.size();i++)
    {
        //获取

        QJsonObject getObj = textArray.at(i).toObject();
        QString commodity = getObj.value("商品").toString();
        float InventorNumber = getObj.value("库存数量").toDouble();
        float unitPrice =getObj.value("单件价格").toDouble();
        commodityList<<commodity;

        //显示
        classData cd;
        cd.ok=true;
        cd.commodity = commodity;qDebug()<<cd.commodity;
        cd.com_num = InventorNumber;qDebug()<<cd.com_num<<""<<InventorNumber;
        cd.price = unitPrice;qDebug()<<cd.price<<""<<unitPrice;
        QListWidgetItem *item = new  QListWidgetItem ();
        Goods *goodWidget = new Goods();
        goodWidget->set(cd);
        item->setSizeHint(QSize(433,45));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,goodWidget);
    }
    ui->comboBox_commodity->addItems(commodityList) ;
}

//返回
void ManagerGoods::reutnrnslots()
{
    Manager * m = new Manager();
    m->show();
    this->close();
}

/**
 * @brief  表头
 * @param
 * @return
**/
void ManagerGoods::showHead()
{
    //创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    GoodsCheck * gc = new GoodsCheck();

    //第二点：设置条目窗口内容
    classData cd ;
    cd.ok = false ;

    gc->set_user_shopping(cd);
    //第三点：很重要
    ui->listWidget_2->addItem(item); // 添加条目
    item->setSizeHint(QSize(650,47)); //最好要设置窗口的大小一致
    ui->listWidget_2->setItemWidget(item,gc);//条目窗口显示
}

/**
 * @brief   ui->listWidget_2 --- 销售情况的查询
 * @param
 *          QString text --- 订单的名称
 * @return
**/
void ManagerGoods::showListwidget(QString text)
{
    ui->listWidget_2->clear();
    showHead();

    //1.获取到该购物清单的json对象
    QJsonObject commodObj =  read_obj(shopDoc);
    QJsonArray  fristArray = commodObj.value("购物清单").toArray();
    QJsonObject senondObj;
    for(int i=0;i<fristArray.size();i++)
    {
        senondObj = fristArray.at(i).toObject();
        if( text.toInt() == senondObj.value("订单编号").toInt())
        {
            break;
        }
    }

    //2.该购物清单的内容
    QString date = senondObj.value("采购日期").toString();
    QString name = senondObj.value("收银员").toString();
    QJsonObject typeObj =senondObj.value("采购种类").toObject();
    QStringList list_type = typeObj.keys();
    for(int i=0;i<list_type.size();i++)
    {
        QString typeName = list_type.at(i);//商品种类
        QJsonArray type_array = typeObj.value(typeName).toArray();
        for(int j=0;j<type_array.size();j++)
        {

            //3.显示到listwidget_2

            QJsonObject obj = type_array.at(j).toObject();
            float ShippingPrice =  obj.value("金额").toDouble();
            float PurchasePrice = obj.value("单价").toDouble();
            float num = obj.value("数量").toDouble();
            QString commodity =  obj.value("商品").toString();

            classData cd1;
            cd1.ok = true;
            cd1.ShippingPrice = ShippingPrice;//金额
            cd1.PurchasePrice = PurchasePrice; //单价
            cd1.com_num = num;
            cd1.Date = date;
            cd1.name = name;
            cd1.typeText = typeName;
            cd1.commodity = commodity;

            QListWidgetItem *item = new  QListWidgetItem ();
            GoodsCheck * gc = new GoodsCheck();
            gc->set_user_shopping(cd1);
            item->setSizeHint(QSize(650,47));
            ui->listWidget_2->addItem(item);
            ui->listWidget_2->setItemWidget(item,gc);
        }
    }
}



