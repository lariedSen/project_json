#ifndef LAND_H
#define LAND_H

#include <QWidget>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>


namespace Ui {
class Land;
}

class Land : public QWidget
{
    Q_OBJECT

public:
    explicit Land(QWidget *parent = nullptr);
    ~Land();

    void newMan();



signals:
    void sendName(QString text);

private slots:
    void on_londBtn_clicked();

    void on_exitBtn_clicked();



private:
    Ui::Land *ui;
    int num ;
    int totalnum;

    QFile filenew;
    QJsonArray array_people;

};

#endif // LAND_H
