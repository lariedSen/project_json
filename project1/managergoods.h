#ifndef MANAGERGOODS_H
#define MANAGERGOODS_H

#include <QWidget>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QMessageBox>
#include<QTimer>
#include"classData.h"

#include<QFileInfo>
#include<QListWidgetItem>


namespace Ui {
class ManagerGoods;
}

class ManagerGoods : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerGoods(QWidget *parent = nullptr);
    ~ManagerGoods();

    //设置表头
    void setHeadCheak();
    void setHead();

    //对文件的读取
    QJsonObject read_obj(QString pathFile);
    QJsonObject getInfo(QJsonArray js_array , QString commodity);
    void  writeJsonFile(QJsonObject include_obj, QString pathFile);


    QStringList getCombox_user();
    void showHead();

signals:
   void comboxTextchange(QString text);
   void UserListwidget(QString text);

private slots:
    void on_purchaseBtn_clicked();
    void on_pushButton_check_clicked();
    void comboxType(QString text);
    void reutnrnslots();
    void showListwidget(QString text);
    void on_purchaseBtn_finish_clicked();

private:
    Ui::ManagerGoods *ui;
    QString pathCommod;
    QString purcharseList;
    QString tempList;
    QString shopDoc ;



};

#endif // MANAGERGOODS_H
