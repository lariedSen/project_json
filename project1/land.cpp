#include "land.h"
#include "ui_land.h"
#include"mainwindow.h"
#include<QMessageBox>
#include<QFile>
#include<QDebug>
#include<manager.h>
#include<QInputDialog>
#include <QRegExpValidator>
#include"settleaccount.h"
Land::Land(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Land)
{
    ui->setupUi(this);
    num =0;
    totalnum = 0;

    //正则表达式限制输入
    QRegExp regExp("^[A-Za-z0-9]+$");
    ui->PassLineEdit->setValidator(new QRegExpValidator(regExp, this));
    connect(ui->PassLineEdit,&QLineEdit::returnPressed,this,&Land::on_londBtn_clicked);


}

Land::~Land()
{
    delete ui;
}

/**
 * @brief  登陆功能
 * @param
 * @return
**/
void Land::on_londBtn_clicked()
{

    //确定输入的次数
    num++;
    //获取输入的对
    QString get_Account =   ui->AccoutLineEdit->text();
    QString get_Password =   ui->PassLineEdit->text();

    ui->AccoutLineEdit->clear();
    ui->PassLineEdit->clear();
    //读取Json文件
    QFile file("../project1/json/AccountManager.json");
    file.open(QIODevice::ReadOnly);
    QByteArray array_read  = file.readAll();
    QJsonDocument doc =  QJsonDocument::fromJson(array_read);
    QJsonObject obj =    doc.object();
    QJsonObject personObj = obj.value("person").toObject();

    //管理员
    QJsonArray Ad_listArray =  personObj.value("Ad_list").toArray();
   //员工用户
    QJsonArray Staff_listArray =  personObj.value("Staff_list").toArray();

    totalnum = Ad_listArray.size()+Staff_listArray.size();



    //读取其中的数据并保存到 字节数组中
    QStringList listAccount; //零时数据，读取出来的所有的账户

    //管理员登陆
      for(int i = 0 ;i<Ad_listArray.size();i++)
      {
          QJsonObject list_obj =   Ad_listArray.at(i).toObject();
          QString Account = list_obj.value("Account").toString();
          QString password = list_obj.value("password").toString();
          listAccount<<Account;

          if(Account ==get_Account && password == get_Password )
          {
            Manager * m = new Manager();
            m->show();
            file.close();
            this->close();
              return;
      }
    }
     //员工登陆
      for (int i =0 ;i<Staff_listArray.size();i++)
      {
          QJsonObject list_obj =   Staff_listArray.at(i).toObject();
          QString Account = list_obj.value("Account").toString();
          QString password = list_obj.value("password").toString();
          listAccount<<Account;
          if(Account ==get_Account && password == get_Password )
          {
               SettleAccount *s = new SettleAccount();
              connect(this,&Land::sendName,s,&SettleAccount::getName);
              emit this->sendName(get_Account);
               s->show();
               file.close();
               this->close();
              return;
          }
      }

      //如果只是密码错误
      for(int i = 0 ;i<listAccount.count();i++)
      {
          if(listAccount.at(i) == get_Account)
          {
              QMessageBox::critical(this,tr("错误提示"),tr("pass error"),QMessageBox::Ok);
              return;
          }
      }
      newMan();
 }



/**
 * @brief   录入注册人员信息
 * @param
 * @return
**/
void Land::newMan()
{

    //如果根本就没有这个人的账户,给他录入信息
    //新用户
    QFile filenew("../project1/json/new.json");
    filenew.open(QIODevice::ReadWrite);

    QMessageBox::StandardButton choice=  QMessageBox::information(this,tr("提示"),
                               QString("你好，你不在我们的数据管理系统\n "
                                       "现在正给你录入游客信息，默认密码111111\n"
                                       "按yes,为你录入，No则退出"),QMessageBox::No|QMessageBox::Yes);

    if(choice == QMessageBox::No)
    {
        MainWindow * m = new MainWindow();
        m->show();
        this->close();
        return;
    }
   if(choice == QMessageBox::Yes)
     {

       bool ok;
       QString text= QInputDialog::getText(this,tr("信息录入"),tr("请输入你的账户\n 默认密码111111"),
                                           QLineEdit::Normal,"tourist",&ok);
       //首次录入
       if(filenew.size()==0)
       {

           if(ok&& !text.isEmpty())
           {
               //最里层对象包
               QJsonObject data_a;
               data_a.insert("Account",text);
               data_a.insert("password","111111");

               //次一级数组包
               QJsonArray array_next ;
               array_next.append(data_a);

               //顶层层对象包
               QJsonObject nextobj;
               nextobj.insert("person",array_next);
               nextobj.insert("totalcount",1);
               //写入文件
               QJsonDocument doc;
               doc.setObject(nextobj);
               QByteArray data = doc.toJson();
               filenew.write(data);
               filenew.close();
           }
           else{
                qDebug()<<"$%^**********************************";
               QMessageBox::critical(this,tr("错误提示"),tr("回去代码看看吧\n 出错了"));
           }

       }
       //文件不为空，里面有数据
       else
       {
           QByteArray array_read  = filenew.readAll();
           QJsonDocument doc =  QJsonDocument::fromJson(array_read);
           filenew.close();

           //解析
            QJsonObject obj =    doc.object();
            int a  = obj.value("totalcount").toInt();
            QJsonArray person_array = obj.value("person").toArray();

            //数据写入
            QJsonObject data_a;
            data_a.insert("Account",text);
            data_a.insert("password","111111");
            for(int i =0 ;i<person_array.size();i++)
            {
                QJsonObject obj = person_array.at(i).toObject();
               array_people.append(obj);
            }
            array_people.append(data_a);

            //情况文档
            QFile filenew("../project1/json/new.json");
            if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
            {
               QMessageBox::warning(this,tr("提示"),tr("清空失败"));
            }

            if(ok)
            {
                //顶层层对象包
                QJsonObject include_obj;
                include_obj.insert("person", array_people);
                include_obj.insert("totalcount",a+1);
                //写入文件
                QJsonDocument docDecu;
                docDecu.setObject(include_obj);
                QByteArray data = docDecu.toJson();
                filenew.write(data);
                filenew.close();
            }
       }
   }

   //如果3次以上就退出
   if(num >= 3)
   {
       num = 0;
       MainWindow * m = new MainWindow();
       m->show();
       this->close();
   }

}


/**
 * @brief  退出程序
 * @param
 * @return
**/
void Land::on_exitBtn_clicked()
{
    MainWindow * m = new MainWindow();
    m->show();
    this->close();
}
