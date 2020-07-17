#ifndef DELETESTAFFITEM_H
#define DELETESTAFFITEM_H

#include <QWidget>
#include"classData.h"
namespace Ui {
class DeleteStaffitem;
}

class DeleteStaffitem : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteStaffitem(QWidget *parent = nullptr);
    ~DeleteStaffitem();
    void set(classData &cd);

private:
    Ui::DeleteStaffitem *ui;
};

#endif // DELETESTAFFITEM_H
