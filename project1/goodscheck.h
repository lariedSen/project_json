#ifndef GOODSCHECK_H
#define GOODSCHECK_H

#include <QWidget>
#include"classData.h"
namespace Ui {
class GoodsCheck;
}

class GoodsCheck : public QWidget
{
    Q_OBJECT

public:
    explicit GoodsCheck(QWidget *parent = nullptr);
    ~GoodsCheck();

    void set(classData &cd);
    void set_seettelAccout(classData&cd);
    void set_user_shopping(classData&cd);

private:
    Ui::GoodsCheck *ui;
};

#endif // GOODSCHECK_H
