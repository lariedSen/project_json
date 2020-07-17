#include "managerstaff.h"
#include "ui_managerstaff.h"
#include"manager.h"
#include<QtDebug>
#include<QListWidgetItem>
#include"deletestaffitem.h"
#include"classData.h"

ManagerStaff::ManagerStaff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerStaff)
{
    ui->setupUi(this);

    //1.路径
    filePath = "../project1/json/AccountManager.json";

    //2.定时器返回
    timer =new QTimer(this);
    connect(timer,&QTimer::timeout,this,&ManagerStaff::return_next_show);

    //3.只能输入数字
    QRegExp regExp("^[0-9]*$");
    ui->PassLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->deletelEdit->setValidator(new QRegExpValidator(regExp, this));

    //4.listWidget 的头文字设置
    initDeleteWidget();
    on_updataBtn_clicked();

}

ManagerStaff::~ManagerStaff()
{
    delete ui;
}

void ManagerStaff::initDeleteWidget()
{
    /*listWidget的使用  **/

     //创建条目 + 条目窗口
     QListWidgetItem *item = new  QListWidgetItem ();
     DeleteStaffitem *de_item = new DeleteStaffitem();

     //第二点：设置条目窗口内容
     classData cd ;
     cd.ok = true ;
     cd.account = "账号";
     cd.password = "密码";
     de_item->set(cd);

     //第三点：很重要
     ui->listWidget->addItem(item); // 添加条目
     item->setSizeHint(QSize(350,50)); //最好要设置窗口的大小一致
     ui->listWidget->setItemWidget(item,de_item);//条目窗口显示
}

/**
 * @brief  获取里面的数据（最外面的大括号对象数据）
 *  Json数据文档通用
 * @param
 * @return
**/
QJsonObject ManagerStaff::read_array(QString pathFile)
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
 * @brief  把QJsonObject对象的数据写入到文件中
 * @param
 * @return
**/
void ManagerStaff::writeJsonFile(QJsonObject include_obj, QString pathFile)
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
    else {
         QMessageBox::information(this,tr("温馨提示"),tr("操作完成"),QMessageBox::Yes);
    }
}

/**
 * @brief  读取文本所有的“key”的数据，
 * @param  Ad_array -- 管理员数组数据，
 *          Staff_array  -- 员工的数组数据
 * @return  QStringList -- key的数据链表
**/
QStringList ManagerStaff::readData(QJsonObject personObj, QString key)
{
    QJsonArray Ad_array = personObj.value("Ad_list").toArray();
    QJsonArray Staff_array = personObj.value("Staff_list").toArray();

    QStringList List;
    for(int i = 0;i<Ad_array.size();i++)
    {
      QJsonObject obj_acc =  Ad_array.at(i).toObject();
      List<<obj_acc.value(key).toString();
    }
    for(int i = 0;i<Staff_array.size();i++)
    {
      QJsonObject obj_acc =  Staff_array.at(i).toObject();
      List<<obj_acc.value(key).toString();
    }

    return List ;
}




/**
 * @brief增加收银员
 * @param
 * @return
**/
void ManagerStaff::on_addBtn_clicked()
{


    //0.先获取输入的内容
    QString getaccount =  ui->AccountLineEdit->text();
    QString getpassword = ui->PassLineEdit->text();

    //1. 获取数据
    QJsonObject obj = read_array(filePath);

    QJsonObject personObj = obj.value("person").toObject();
    QJsonArray Ad_array = personObj.value("Ad_list").toArray();
    QJsonArray Staff_array = personObj.value("Staff_list").toArray();


    //2.获取账户进行判断
    QStringList accountList =readData(personObj,"Account");


    for(int i =0;i<accountList.size();i++)
    {
        if(getaccount == accountList.at(i))
        {
            QMessageBox::warning(this,tr("温馨提示"),tr("该账户已经注册！\n 请更换"),QMessageBox::Ok);
            return ;
        }
    }

    //3.1新增的数据添加到对应的数据
    QJsonObject data_a;
    data_a.insert("Account",getaccount);
    data_a.insert("password",getpassword);
    Staff_array.append(data_a);

    //3.2数据层层包
    QJsonObject person ;
    person.insert("Ad_list",Ad_array);
    person.insert("Staff_list",Staff_array);
    QJsonObject include_obj;
    include_obj.insert("totalcount",readData(person,"Account").size());
    include_obj.insert("person", person);

    //3.3把数据写入到文件中
    writeJsonFile(include_obj, filePath);
    timer->start(2000);
    tits();

    ui->AccountLineEdit->clear();
    ui->PassLineEdit->clear();
}


/**
 * @brief  窗口提示
 * @param
 * @return
**/
void ManagerStaff::tits()
{
    dialog = new QDialog();
    dialog->setWindowTitle(tr("提示"));
    QLabel * label = new QLabel("2秒后，返回管理员主界面");
    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(label);
    dialog->setLayout(layout);
    dialog->resize(200,100);
    dialog->show();
}


/**
 * @brief  删除指定收银员
 * @param
 * @return
**/
void ManagerStaff::on_deleteBtn_clicked()
{
    //1.读取数据
    QJsonObject obj=  read_array(filePath) ;
    QJsonObject personObj = obj.value("person").toObject();
    QJsonArray Ad_array = personObj.value("Ad_list").toArray();
    QJsonArray Staff_array = personObj.value("Staff_list").toArray();

    //2.获取该成员所在的数json数组
    int choice =  ui->deletelEdit->text().toInt();qDebug()<<choice;
    if(choice > Staff_array.size())
    {
        QMessageBox::warning(this,tr("提示"),QString("你的数值太大\n请输入小于"));
        return ;
    }
    for(int i = 0;i<Staff_array.size();i++)
    {
      QJsonObject obj_acc =  Staff_array.at(i).toObject();
       QString account = obj_acc.value("Account").toString();

       if(i+1 == choice)
       {
          //可以开始删除
          Staff_array.removeAt(i);
          break;
       }
   }

   //3.把数据写入到文件中
   QJsonObject person ;
   person.insert("Ad_list",Ad_array);
   person.insert("Staff_list",Staff_array);
   QJsonObject include_obj;
   include_obj.insert("person", person);
   include_obj.insert("totalcount",readData(person,"Account").size());
   writeJsonFile(include_obj, filePath);
   ui->deletelEdit->clear();

}

//****************************************************
//界面跳转
void ManagerStaff::on_returnBtn_clicked()
{
    Manager * m = new Manager();
    m->show();
    this->close();
}
void ManagerStaff::return_next_show()
{
    timer->stop();
    dialog->close();
    Manager * m = new Manager();
    m->show();
    this->close();
}
//*****************************************************


//====================槽函数
/**
 * @brief  刷新 listwidget 的显示
 * @param
 * @return
**/
void ManagerStaff::on_updataBtn_clicked()
{
    ui->listWidget->clear();
    //1.读取Json文件数据
    QJsonObject obj = read_array(filePath);

    QJsonObject personObj = obj.value("person").toObject();
    int a = obj.value("totalcount").toInt();
    QJsonArray Ad_array = personObj.value("Ad_list").toArray();
    QJsonArray Staff_array = personObj.value("Staff_list").toArray();

    //2.读取文本的员工的帐号 和密码
    //QStringList accoutList;
    for(int i = 0;i<Staff_array.size();i++)
    {
      QJsonObject obj_acc =  Staff_array.at(i).toObject();
      QString account = obj_acc.value("Account").toString();
      QString pass = obj_acc.value("password").toString();

        QListWidgetItem *item = new  QListWidgetItem ();
        DeleteStaffitem *de_item = new DeleteStaffitem();

        ui->listWidget->addItem(item);

        classData cd;
        cd.ok = false ;
        cd.num = i+1;
        cd.account = account;qDebug()<<cd.account;
        cd.password =pass;qDebug()<<cd.password;
        de_item->set(cd);

        item->setSizeHint(QSize(350,50));
        ui->listWidget->setItemWidget(item,de_item);
    }
}
