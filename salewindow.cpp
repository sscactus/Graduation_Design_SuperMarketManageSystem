#include "salewindow.h"
#include "ui_salewindow.h"

saleWindow::saleWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::saleWindow)
{
    ui->setupUi(this);
}

saleWindow::~saleWindow()
{
    delete ui;
}
