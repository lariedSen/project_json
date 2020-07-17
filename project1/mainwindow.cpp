#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"land.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_OnBtn_clicked()
{
    Land * l =new Land();
    l->show();
    this->close();
}

void MainWindow::on_AppCloseBtn_clicked()
{
    QApplication* app;
     app->exit(0);
}
