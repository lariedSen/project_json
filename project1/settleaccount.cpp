#include "settleaccount.h"
#include "ui_settleaccount.h"
#include"goodscheck.h"
#include<QDebug>
#include"goods.h"
#include<QDateTime>
#include"land.h"

/*************************************************
Copyright:
Author:Laried
Date to update:2020年7月16日
Description (描述主要实现的功能):
[1] 员工结算，商品入库。
    （1）管理员的采购清单 :有单个清单结算 和 一键全部结算

[2]员工对客户购买的商品检索。
    （1）输入数量，结算金额。
    （2）并写入到销售记录中

[3] 查询销售的记录
**************************************************/

SettleAccount::SettleAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettleAccount)
{
    ui->setupUi(this);

    //1.员工登陆显示Label_2
    QString showName = QString("欢迎你登陆百慕大商场系统，员工%1。祝你生活愉快！").arg(name_man);
    ui->label_2->setText(showName);

    //2.预先设置路径
    pathCommod = "../project1/json/commodity.json"; //这个路径没用的
    purcharseList = "../project1/json/PurchaseList.json";
    shopMall ="../project1/json/ShoppingMall.json" ;
    shopDoc = "../project1/json/ShopDoc.json";
    tempList = "../project1/json/tempPur.json";

    //3.清空临时文档
    QFile filenew(tempList);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {

    }
    filenew.close();

    //4.正则表达式限制输入
    QRegExp regExp("[0-9.0-9]*");
    ui->lineEdit_num->setValidator(new QRegExpValidator(regExp, this));

    //5.返回上一级界面
    connect(ui->pushButton_return,&QAbstractButton::clicked,this,&SettleAccount::return_slots);
    connect(ui->pushButton_retnrn2,&QAbstractButton::clicked,this,&SettleAccount::return_slots);

   //6.1客户购物的comboBox_type初始化
    QJsonObject commodObj =  read_Obj(shopMall);
    QJsonObject typeObj = commodObj.value("type").toObject();
    QStringList list = typeObj.keys();
    ui->comboBox_type->addItems(list);
    for(int i=0 ;i<list.size();i++)
    {
        qDebug()<<list.at(i);
    }

    //6.2客户购物的，listwidget_user实时显示
    connect(ui->comboBox_type,&QComboBox::currentTextChanged,this,&SettleAccount::typeListwidgetSlots);
    QString text = ui->comboBox_type->currentText();
    connect(this,&SettleAccount::typeListWidgetCommdity,this,&SettleAccount::typeListwidgetSlots);
    emit this->typeListWidgetCommdity(ui->comboBox_type->currentText());
    ui->pushButton_sellte_user->setEnabled(false);


    connect(this,&SettleAccount::showlist2,this,&SettleAccount::showListwidget);

    //7.1员工结算：管理员的采购清单，
    //comboBox变化，界面实时更新
    connect(ui->comboBox,&QComboBox::currentTextChanged,this,&SettleAccount::listWidgetShow);
    connect(this,&SettleAccount::listwidget_show,this,&SettleAccount::listWidgetShow);
    //7.2设置combox的值
    QStringList getlist1 =  getcombox();
    if(!getlist1.isEmpty())
    emit this->listwidget_show(ui->comboBox->currentText());


    //8.1设置comboBox_OrdCheck的值,员工查询实时更新
    QStringList getList2 = getCombox_user();
    //8.2员工查询实时更新
    connect(ui->comboBox_OrdCheck,&QComboBox::currentTextChanged,this,&SettleAccount::showListwidget);
    connect(this,&SettleAccount::UserListwidget,this,&SettleAccount::showListwidget);
    if(!getList2.isEmpty())
    {
         emit this->UserListwidget(ui->comboBox_OrdCheck->currentText());
    }



}

SettleAccount::~SettleAccount()
{
    delete ui;
}


/**
 * @brief  员工结算的头文字数据
 * @param
 * @return
**/
void SettleAccount::setHeadCheak()
{
    //qDebug()<<"void SettleAccount::setHeadCheak()";
    //1.创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    GoodsCheck * gc = new GoodsCheck();

    //2.设置条目窗口内容
    classData cd ;
    cd.ok = false ;
    gc->set_seettelAccout(cd);

    //3.添加条目和窗口显示
    ui->listWidget->addItem(item);
    item->setSizeHint(QSize(650,47)); //最好要设置窗口的大小一致
    ui->listWidget->setItemWidget(item,gc);//条目窗口显示

}

/**
 * @brief  获取没有付款的订单的编号并设置到combox，
 * @param
 * @return  字符串列表:ui->comboBox的值
**/
QStringList SettleAccount::getcombox()
{
    //1.combox的变化会影响到界面的更新，----（bug搞死）
    ui->comboBox->disconnect();

    //2.获取没有付款的订单的编号
    ui->comboBox->clear();
    QJsonObject big_Obj =  read_Obj(purcharseList);
    QJsonArray big_array = big_Obj.value("采购清单").toArray();
    QStringList num_list;
    for(int i =0 ;i<big_array.size();i++)
    {
        QJsonObject read_obj = big_array.at(i).toObject();
        bool pay =  read_obj.value("是否付款").toBool();
        if(!pay)
        {
            int  Order_num = read_obj.value("订单编号").toInt();
            num_list<<QString::number(Order_num);
        }
    }
    //3.实时显示最新的combox的值
    if(!num_list.isEmpty())
    {
        ui->comboBox->addItems(num_list);
        emit this->listwidget_show(ui->comboBox->currentText());
        connect(ui->comboBox,&QComboBox::currentTextChanged,this,&SettleAccount::listWidgetShow);
        return num_list;
    }
    else{
        ui->comboBox->clear();
        ui->listWidget->clear();
        connect(ui->comboBox,&QComboBox::currentTextChanged,this,&SettleAccount::listWidgetShow);
        return num_list;
    }


}


/**
 * @brief： 购物的combox 设置
 * @param
 * @return  字符串列表:ui->comboBox_OrdCheck的值
**/
QStringList SettleAccount::getCombox_user()
{
    ui->comboBox_OrdCheck->clear();
    QJsonObject big_Obj =  read_Obj(shopDoc);
    QJsonArray big_array = big_Obj.value("购物清单").toArray();
    QStringList num_list;
    for(int i =0 ;i<big_array.size();i++)
    {
        QJsonObject read_obj = big_array.at(i).toObject();
        int  Order_num = read_obj.value("订单编号").toInt();
        num_list<<QString::number(Order_num);

    }
    ui->comboBox_OrdCheck->addItems(num_list);
    return num_list;
}


/**
 * @brief  设置查询窗口listwidget 的头文字数据
 * @param
 * @return
**/
void SettleAccount::showHead()
{
    //1. 创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    GoodsCheck * gc = new GoodsCheck();

    //2.：设置条目窗口内容
    classData cd ;
    cd.ok = false ;

    gc->set_user_shopping(cd);
    //3.添加条目
    ui->listWidget_2->addItem(item); //
    item->setSizeHint(QSize(650,47)); //最好要设置窗口的大小一致
    ui->listWidget_2->setItemWidget(item,gc);//条目窗口显示
}


/**
 * @brief   接口函数：
 *          [1]员工传递过来的一个订单编号数值
 * @param
 * @return  [2]返回该订单编号所在的一个对象
**/
QJsonObject SettleAccount::getOeder(int Ordnum)
{
    QJsonObject big_Obj =  read_Obj(purcharseList);
    QJsonArray big_array = big_Obj.value("采购清单").toArray();
    for(int i =0 ;i<big_array.size();i++)
    {
        QJsonObject read_obj = big_array.at(i).toObject();
        bool pay =  read_obj.value("是否付款").toBool();qDebug()<<pay;
        if(!pay && Ordnum == read_obj.value("订单编号").toInt())
        {
            qDebug()<<Ordnum<<read_obj.value("订单编号").toInt();
            return  read_obj;
        }
    }
}




/**
 * @brief  槽函数：界面listwidget的更新显示
 *         [1]得到对应的text值。
 *         [2]根据text值，去获取该订单编号所在的一个对象
 *         [3]得到该对象，进行json解析，然后更新到listwidget
 *
 * @param text 用于获取该值所对应的对象
 * @return
**/
void SettleAccount::listWidgetShow(QString text)
{
    if(text.isEmpty())
    {
        ui->listWidget->clear();
        QMessageBox::warning(this,tr("提示"),"void SettleAccount::listWidgetShow(QString text)");
        return ;

    }

    //当前订单金额清零
     thisMoney =0;

    //清空且设置头数据
    ui->listWidget->clear();
    QJsonObject read_obj = getOeder(text.toInt());
    setHeadCheak();

    float currOederMoney=0;
    QString date= read_obj.value("采购日期").toString();
    QJsonObject typeObj = read_obj.value("采购种类").toObject();
    //获取采购种类的内容
    QStringList typeCommodityList =  typeObj.keys();
    for(int i=0;i<typeCommodityList.size();i++)
    {
        //某一个种类：例如冷冻
        QJsonArray typeArray = typeObj.value(typeCommodityList.at(i)).toArray();
        for(int j =0;j<typeArray.size();j++)
        {
            //例如冷冻类型的某一个对象 的数据
            QJsonObject obj = typeArray.at(j).toObject();
            float PurchasePrice = obj.value("进货价格").toDouble();
            float num = obj.value("采购数量").toDouble();
            QString commodity =  obj.value("商品").toString();
            QString man = obj.value("采购人").toString();
            float ShippingPrice = PurchasePrice * num; //该商品的金额
            currOederMoney+=ShippingPrice;//该种类结算时的总金额

            classData cd;
            cd.ok = true;
            cd.ShippingPrice = ShippingPrice;
            cd.PurchasePrice = PurchasePrice;
            cd.com_num = num;
            cd.Date = date;
            cd.name = man;
            cd.typeText = typeCommodityList.at(i);
            cd.commodity = commodity;
            QListWidgetItem *item = new  QListWidgetItem ();
            GoodsCheck * gc = new GoodsCheck();
            gc->set_seettelAccout(cd);
            item->setSizeHint(QSize(650,47));
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item,gc);

        }

    }

    //当前订单金额获取
    thisMoney =currOederMoney ;
    totalMoney+=thisMoney;

}


/**
 * @brief   listWidget_user 设置头文字
 * @param
 * @return
**/
void SettleAccount::showUserHead()
{
    //创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    Goods * gs = new Goods();

    //第二点：设置条目窗口内容
    classData cd ;
    cd.ok = false ;
    //gc->set(cd);
    gs->setUser(cd);
    //
    ui->listWidget_user->addItem(item); // 添加条目
    item->setSizeHint(QSize(433,45)); //最好要设置窗口的大小一致
    ui->listWidget_user->setItemWidget(item,gs);//条目窗口显示
}




//================ 接口函数
/**
 * @brief   获取该文件的内容，并返回最外层的大括号对象
 * @param   文件路径
 * @return   QJsonObject ---
**/
QJsonObject SettleAccount::read_Obj(QString pathFile)
{
    QFile file(pathFile);
    file.open(QIODevice::ReadOnly);
    QByteArray array_read  = file.readAll();
    QJsonDocument doc =  QJsonDocument::fromJson(array_read);
    QJsonObject obj = doc.object();
     file.close();
     return obj;
}

//================ 接口函数
/**
 * @brief  把QJsonObject对象的数据写入到文件中
 * @param
 *          QJsonObject include_obj --- 此对象
 *          QString pathFile    --------该文件的路径
 * @return
**/
void SettleAccount::writeJsonFile(QJsonObject include_obj, QString pathFile)
{
        //清空
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
}

//-----------------------
// 跨窗口界面的数据传递
// 原理：信号与槽
//-----------------------
void  SettleAccount::getName(QString name)
{
   name_man =name;
}

/**
 * @brief  返回主界面
 * @param
 * @return
**/
void SettleAccount::return_slots()
{

    Land * m = new Land ();
    m->show();
    this->close();


}

//==============接口函数
/**
 * @brief  员工结算 订单编号为num 的订单
 *
 * @param  int num  订单编号为num
 * @return
**/
void SettleAccount::SettlementProcurement(int num)
{

    //1.读出3个数据：采购数量，出货价格，商品名称
    QJsonObject read_obj = getOeder(num);
    QJsonObject typeObj = read_obj.value("采购种类").toObject();
    int  Ord_num =read_obj.value("订单编号").toInt();
    QStringList typeCommodityList =  typeObj.keys();

    //2.单个对象的金额
    float money;

    //3.结算该对象的数据，并把数据按数据模型写入到 ShoppingMall.json文件
    for(int i=0;i<typeCommodityList.size();i++)
     {
        //3.1 类型 json数组
         QJsonArray typeArray = typeObj.value(typeCommodityList.at(i)).toArray();
         for(int j =0;j<typeArray.size();j++)
         {
             //3.2 该类型的某一个对象
             QJsonObject obj = typeArray.at(j).toObject();

             //3.3 所需要数据写入到ShoppingMall.json
             float ShippingPrice = obj.value("出货价格").toDouble();
             float getnum = obj.value("采购数量").toDouble();
             QString commodity =  obj.value("商品").toString();
             money= ShippingPrice*getnum;

             //3.4.把当前商品数据写入到ShoppingMall.json ，
             setShopMall(Ord_num,j,typeCommodityList.at(i),commodity,getnum,ShippingPrice);
         }

     }

    //4.同时把采购的订单状态改为true
    setPurTrue(num);
}

/**
 * @brief  写入到ShoppingMall.json
 * @param
 * @return
**/
void SettleAccount::setShopMall(int Ord_num, int j,QString typeText,
                                QString commodity, float getnum , float ShippingPrice )
{

       //1. 判断是否位空文件
        QFile file(shopMall);
        file.open(QIODevice::ReadOnly);
        QJsonObject typeObj ;
        QJsonArray arrayType ;
        if(file.size() == 0)
        {
            //1.1 要新写入的商品购买数据
            QJsonObject goods ;
            goods.insert("数量",getnum);
            goods.insert("商品",commodity);
            goods.insert("价格",ShippingPrice);

            file.close();
            arrayType.prepend(goods);
            typeObj.insert(typeText,arrayType);
        }
        else{
            file.close();

            QJsonObject big_obj = read_Obj(shopMall);
            typeObj =  big_obj.value("type").toObject();
            arrayType =  typeObj.value(typeText).toArray();
            for(int i =0 ;i<arrayType.size();i++)
            {
                //创建临时对象
                QJsonObject temp_obj=arrayType.at(i).toObject();
               if( temp_obj.value("商品").toString() == commodity)
               {
                   float price  = temp_obj.value("价格").toDouble();
                   if(ShippingPrice == price)
                   {
                       //*************************************
                       // 最初测试时，这是个bug,不是代码错误，而是逻辑冲突，
                       //解决方法：同名商品但是价格相同，那么把它们的数量直接相加
                       //*************************************
                       float num =  temp_obj.value("数量").toDouble();
                       arrayType.removeAt(i);
                       QJsonObject goods ;
                       goods.insert("数量",getnum+num);
                       goods.insert("商品",commodity);
                       goods.insert("价格",ShippingPrice);
                       arrayType.prepend(goods);
                       typeObj.remove(typeText);
                       typeObj.insert(typeText,arrayType);

                       //数据外层次包装
                       QJsonObject include_obj;
                       include_obj.insert("type",typeObj);
                       //写入到临时文件中
                       writeJsonFile(include_obj,shopMall);
                       return ;
                   }

                   //*************************************
                   // 最初测试时，这是个bug,不是代码错误，而是逻辑冲突，
                   //解决方法：同名商品但是价格不同，应该区分商品的名字，加入标注
                   //*************************************
                   //如果是同名商品但是价格不同，应该区分
                   commodity = QString("%1_%2_%3").arg(commodity).arg(Ord_num).arg(j);
                  i=-1;
               }

           }

            //数据打包
            QJsonObject goods ;
            goods.insert("数量",getnum);
            goods.insert("商品",commodity);
            goods.insert("价格",ShippingPrice);
            arrayType.prepend(goods);
            typeObj.remove(typeText);
            typeObj.insert(typeText,arrayType);
        }

        //包装
        QJsonObject include_obj;
        include_obj.insert("type",typeObj);
        //写入到ShoppingMall.json
        writeJsonFile(include_obj,shopMall);
}

/**
 * @brief  把订单的"是否付""改为true
 * @param  第几号订单
 * @return
**/
void SettleAccount::setPurTrue(int num)
{
    QJsonObject big_obj = read_Obj(purcharseList);
    QJsonArray big_array = big_obj.value("采购清单").toArray();
    for(int i =0 ;i<big_array.size();i++)
    {
        QJsonObject this_obj = big_array.at(i).toObject();

        if( num == this_obj.value("订单编号").toInt())
        {
            QString date =  this_obj.value("采购日期").toString();
            QJsonObject typeObj =  this_obj.value("采购种类").toObject();
            QJsonObject newObj ;
            newObj.insert("是否付款",true);
            newObj.insert("订单编号",num);
            newObj.insert("采购日期",date);
            newObj.insert("采购种类",typeObj);
            big_array.removeAt(i);
            big_array.insert(i,newObj);

            // 写入到文件中
            QJsonObject include_obj;
            include_obj.insert( "采购清单",big_array);
            writeJsonFile(include_obj,purcharseList);

            return ;
        }
    }


}


//==============槽函数
/**
 * @brief  员工结算采购的数据，
 * 确认结算则写入商场的商品.json ，
 * @param
 * @return
**/
void SettleAccount::on_pushButton_settle_clicked()
{

    //***************************
    //自定义消息盒子的设置 ，个人博客https://blog.csdn.net/laried/article/details/107104703
    //***************************
    QMessageBox msgBox;

     msgBox.setWindowTitle("结算");//1.标题
     msgBox.setText(tr("员工你好，是否结算采购订单的商品金额!"));// 2.内容
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No); // 3.按键
     msgBox.setButtonText(QMessageBox::Yes,tr("是"));//4.给按键取别名
     msgBox.setButtonText(QMessageBox::No,tr("否"));
     msgBox.setButtonText(QMessageBox::Cancel,tr("暂不处理"));
     msgBox.setDefaultButton(QMessageBox::Yes); //5.设置默认按键

     //7.执行反应
    int ret = msgBox.exec();//6.模态显示
    switch (ret) {
    case QMessageBox::Yes:
    {

        //函数接口：员工结算订单编号为num 的订单 ，并写入到文件
        SettlementProcurement(ui->comboBox->currentText().toInt());
        QMessageBox::information(this,
                         tr("支付提示"),
                         QString("你选择了支付\n"
                                 "请你支付%1\n"
                                 "请把钱转到我的支付宝131******36").arg(thisMoney),
                                 QMessageBox::Close);

         //重新设置combox的值
         QStringList list = getcombox();
         if(list.isEmpty())
         {
             QMessageBox::warning(this,tr("代码616"),tr("索引已经空了"));
         }
        return ;
    }break;
    case QMessageBox::No:
    {
        QMessageBox::information(this,tr("提示"),tr("你还未付款，\n小心出门被狗咬"),
                                 QMessageBox::Close);
    }break;
    case QMessageBox::Cancel:
    {
        QMessageBox::information(this,tr("提示"),tr("请尽快付款\n亲!!!"),
                                 QMessageBox::Close);
    }break;
    }
}

//==============槽函数
/**
 * @brief   一键结算
 * @param
 * @return
**/
void SettleAccount::on_pushButton_settle_all_clicked()
{
    int ret = QMessageBox::information(this,tr("提示"),tr("你选择了支付\n请把钱转到我的支付宝131******36"),
                             QMessageBox::Yes|QMessageBox::No);

    switch (ret) {
    case QMessageBox::Yes :
    {
         QStringList list =getcombox();
         for (int i =0;i<list.size();i++)
         {
            SettlementProcurement(list.at(i).toInt());
         }
         ui->comboBox->disconnect();
         ui->comboBox->clear();
         ui->listWidget->clear();
         QMessageBox::information(
                     this,tr("提示"),
                     QString("你选择了支付\n 请支付%1 \n"
                             "请把钱转到我的支付宝131******36").arg(totalMoney),
                     QMessageBox::Close);
          connect(ui->comboBox,&QComboBox::currentTextChanged,this,&SettleAccount::listWidgetShow);
    }break;
    case QMessageBox::No :
    {

        QMessageBox::information(this,tr("提示"),tr("你选择了NO，结算未未成功\n亲，请尽快付款!!!"),
                                 QMessageBox::Close);
    }break;
    }

    this->totalMoney =0;


}

//==============槽函数
/**
 * @brief  客户选择商品实时更新视图
 * @param
 * @return
**/
void SettleAccount::typeListwidgetSlots(QString text)
{
    ui->listWidget_user->clear();
    ui->comboBox_commodity->clear();
    qDebug()<<text;
    //读取商品的数据
    QJsonObject commodObj =  read_Obj(shopMall);
    QJsonObject typeObj = commodObj.value("type").toObject();

    QStringList commodityList ;
    QJsonArray textArray = typeObj.value(text).toArray();

    showUserHead();
    for(int i=0;i<textArray.size();i++)
    {
        //获取

        QJsonObject getObj = textArray.at(i).toObject();
        QString commodity = getObj.value("商品").toString();
        float InventorNumber = getObj.value("数量").toDouble();
        float unitPrice =getObj.value("价格").toDouble();
        commodityList<<commodity;

        //显示
        classData cd;
        cd.ok=true;
        cd.commodity = commodity;
        cd.com_num = InventorNumber;
        cd.price = unitPrice;
        QListWidgetItem *item = new  QListWidgetItem ();
        Goods *goodWidget = new Goods();
        goodWidget->set(cd);
        item->setSizeHint(QSize(433,45));
        ui->listWidget_user->addItem(item);
        ui->listWidget_user->setItemWidget(item,goodWidget);
    }
    ui->comboBox_commodity->addItems(commodityList) ;


}

//---------------接口函数
/**
 * @brief  获取类型数组中的对应的commodity所在的json对象
 * @param
 * @return  这个接口好像没什么用
**/
QJsonObject SettleAccount::getInfo(QJsonArray js_array, QString commodity)
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




//==============槽函数
/**
 * @brief  客户购买商品
 * @param
 * @return
**/
void SettleAccount::on_buy_btn_clicked()
{

    //1.获取输入的内容
    QString typeText = ui->comboBox_type->currentText();
    QString purchasText = ui->comboBox_commodity->currentText();
    QString strnum =  ui->lineEdit_num->text();
    float getnum= strnum.toFloat();

    if(strnum.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请不要空白输入"),QMessageBox::Close);
        return ;
    }

    //2.判断库存量是否充足
    //2.1 先获取商品所在的对象
    QJsonObject big_obj = read_Obj(shopMall);
    QJsonObject type_obj =  big_obj.value("type").toObject();
    QJsonArray type_array = type_obj.value(typeText).toArray();
    QJsonObject goods_obj = getInfo(type_array, purchasText);//获取到商品所在的对象
    //2.2 所在对象的库存量是否充足
    QString goods =  goods_obj.value("商品").toString();
    float num = goods_obj.value("数量").toDouble();
    float price =  goods_obj.value("价格").toDouble();
    if(num< getnum)
    {
        QMessageBox::warning(this,tr("温馨提示"),
                             tr("很抱歉！库余量不足\n 请重新输入要购买的数量"),QMessageBox::Close);
        ui->lineEdit_num->clear();
        return ;
    }


    float User_monty =  price*getnum;
    User_total_momey += User_monty;

    //4. 写入购物清单
    QString Date = QDateTime::currentDateTime().date().toString("yyyy/MM/dd");
    //4.1要新写入的商品购买数据
    QJsonObject goods1 ;
    goods1.insert("数量",getnum);
    goods1.insert("商品",purchasText);
    goods1.insert("单价",price);
    goods1.insert("金额",price*getnum);
   // goods1.insert("收银员",name);

    //3. 更改所在对象的库存量
    num = num-getnum;

    //3.1 被改动的对象，更新到原数组
    QJsonObject newObj;
    newObj.insert("商品",goods);
    newObj.insert("数量",num);
    newObj.insert("价格",price);
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
    writeJsonFile(change_obj,shopMall);




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
            arrayType.append(goods1);
            typeObj.insert(typeText,arrayType);
        }
        else{
            file.close();
            QJsonObject big_obj = read_Obj(tempList);
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


        emit this->typeListWidgetCommdity(ui->comboBox_type->currentText());
        ui->lineEdit_num->clear();

        ui->pushButton_sellte_user->setEnabled(true);

}

//==============槽函数
/**
 * @brief   员工结算客户购买的商品
 * @param
 * @return
**/
void SettleAccount::on_pushButton_sellte_user_clicked()
{

    if(! ui->lineEdit_num->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("已经检索商品  数量 ，\n请确认或者删除"));
        return ;
    }

    //读取临时文件
    QJsonObject frist_obj = read_Obj(tempList);//采购种类
    QJsonObject wantObj = frist_obj.value("采购种类").toObject();


    QJsonObject small_obj ;
    QString Date = QDateTime::currentDateTime().date().toString("yyyy/MM/dd");
    small_obj.insert("采购日期",Date);
    small_obj.insert("收银员",name_man);


    QFile file(shopDoc);
    file.open(QIODevice::ReadOnly);
    if(file.size() == 0)
    {
        small_obj.insert("订单编号",1000);
        small_obj.insert("采购种类",wantObj);
        QJsonArray middle_array;
        middle_array.prepend(small_obj);
        QJsonObject include_obj ;
        include_obj.insert("购物清单",middle_array);
        writeJsonFile(include_obj,shopDoc);
    }
    else
    {
        QJsonObject big_obj = read_Obj(shopDoc);
        QJsonArray middle_array = big_obj.value("购物清单").toArray();
        small_obj.insert("订单编号",1000+middle_array.size());
        small_obj.insert("采购种类",wantObj);
        middle_array.prepend(small_obj);
        QJsonObject include_obj ;
        include_obj.insert("购物清单",middle_array);
        writeJsonFile(include_obj,shopDoc);
    }

    //清空临时文档
    QFile filenew(tempList);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {

    }
    filenew.close();

    //更新查询
    ui->comboBox_OrdCheck->disconnect();
    QStringList list_ = getCombox_user();
    if(list_.isEmpty())
    QMessageBox::warning(this,tr("代码775"),tr("你给我我一个空的干嘛"));
    emit this->showlist2(ui->comboBox_OrdCheck->currentText());
    connect(ui->comboBox_OrdCheck,&QComboBox::currentTextChanged,this,&SettleAccount::showListwidget);

    QMessageBox::information(this,tr("结算提示"),QString("客户消费共%1元").arg(User_total_momey)
                             ,QMessageBox::Close);

    ui->pushButton_sellte_user->setEnabled(false);
}


//*************注：该函数用不到，已被覆盖
/**
 * @brief  listWidget结算界面 的头文字
 * @param
 * @return
**/
void SettleAccount::UserHeadListwisget()
{
    //创建条目 + 条目窗口
    QListWidgetItem *item = new  QListWidgetItem ();
    GoodsCheck * gc = new GoodsCheck();

    //第二点：设置条目窗口内容
    classData cd ;
    cd.ok = false ;
    gc->set_user_shopping(cd);
    //第三点：很重要
    ui->listWidget->addItem(item); // 添加条目
    item->setSizeHint(QSize(650,47)); //最好要设置窗口的大小一致
    ui->listWidget->setItemWidget(item,gc);//条目窗口显示

}

/**
 * @brief  槽函数：界面listWidget_2的更新显示
 *         [1]得到对应的text值。
 *         [2]   if(text == 某对象的订单编号)
 *                  显示该对象的数据
 *
 * @param text 用于获取该值所对应的对象
 * @return
**/
void SettleAccount::showListwidget(QString text)
{
    ui->listWidget_2->clear();
    showHead();
    QJsonObject commodObj =  read_Obj(shopDoc);
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
            //商品种类中的对象之一
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






