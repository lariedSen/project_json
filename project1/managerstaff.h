#ifndef MANAGERSTAFF_H
#define MANAGERSTAFF_H

#include <QWidget>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QMessageBox>
#include<QTimer>
#include"classData.h"
namespace Ui {
class ManagerStaff;
}

class ManagerStaff : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerStaff(QWidget *parent = nullptr);
    ~ManagerStaff();

    //接口封装
    void initDeleteWidget();//删除的listwidget的头文字显示
    QJsonObject read_array(QString pathFile); //从文件中读取数据
    void writeJsonFile(QJsonObject  include_obj ,QString pathFile);//写入数据到文件
    QStringList readData(QJsonObject personObj,QString key);//读取账户的数据

    //提示返回界面
    void tits();

//按钮槽函数
private slots:
    void on_addBtn_clicked();
    void on_deleteBtn_clicked();
    void on_returnBtn_clicked();
    void return_next_show();
    void on_updataBtn_clicked();

private:
    Ui::ManagerStaff *ui;
    QDialog * dialog;//提示窗口
    QString filePath;//文件路径
    QTimer* timer;//定时器
};

#endif // MANAGERSTAFF_H
