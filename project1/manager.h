#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private slots:
    void on_returnBtn_clicked();

    void on_staffManagerBtn_clicked();

    void on_goodsManagerBtn_clicked();

private:
    Ui::Manager *ui;
};

#endif // MANAGER_H
