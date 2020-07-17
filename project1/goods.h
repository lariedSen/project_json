#ifndef GOODS_H
#define GOODS_H

#include <QWidget>
#include"classData.h"

namespace Ui {
class Goods;
}

class Goods : public QWidget
{
    Q_OBJECT

public:
    explicit Goods(QWidget *parent = nullptr);
    ~Goods();

    void setUser(classData&cd);
     void set(classData &cd);
private:
    Ui::Goods *ui;
};

#endif // GOODS_H
