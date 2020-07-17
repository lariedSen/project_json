#ifndef SETTLEACCOUNT_H
#define SETTLEACCOUNT_H

#include <QWidget>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QMessageBox>

//员工的名字
static  QString name_man ="";

namespace Ui {
class SettleAccount;
}

class SettleAccount : public QWidget
{
    Q_OBJECT

public:
    explicit SettleAccount(QWidget *parent = nullptr);
    ~SettleAccount();


//**************************************************************
//函数接口

    //显示采购
    void setHeadCheak();
    QStringList getcombox();
    QJsonObject getOeder(int Ordnum);//获取订单编号对应的对象

    //显示客户结算
    void showUser();
    void showUserHead();

    //员工结算采购的数据
    void SettlementProcurement(int num);
    void setShopMall(int ,int ,QString ,QString ,float ,float );
    void setPurTrue(int num);

    //读写文件
    QJsonObject read_Obj(QString pathFile);//读出文件内容
    void writeJsonFile(QJsonObject include_obj, QString pathFile); //写入文件

    //员工购物
    QJsonObject getInfo(QJsonArray js_array, QString commodity);
    void UserHeadListwisget();
    QStringList getCombox_user();

    //员工查询
    void showHead();

//***********************************************************************************

signals :
    void listwidget_show(QString text);
    void typeListWidgetCommdity(QString text);
    void UserListwidget(QString text);
    void showlist2(QString text);

public slots :
    void getName(QString name);
private slots:
    void showListwidget(QString text);
    void typeListwidgetSlots(QString text);

    //返回
    void return_slots();

    //按键
    void on_pushButton_settle_clicked();
    void on_pushButton_sellte_user_clicked();

    //员工结算采购的数据的listwidget
    void listWidgetShow(QString text);
    void on_pushButton_settle_all_clicked();
    void on_buy_btn_clicked();

private:
    Ui::SettleAccount *ui;

    //路径
    QString purcharseList;
    QString pathCommod;
    QString shopMall;
    QString shopDoc;
    QString tempList;

    //
    float thisMoney =0 ;
    float totalMoney =0;
    float User_total_momey;
;


};

#endif // SETTLEACCOUNT_H
