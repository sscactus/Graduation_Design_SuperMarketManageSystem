#include "querymerchantdialog.h"
#include "ui_querymerchantdialog.h"
#include "global.h"
#include <QSqlQuery>

QueryMerchantDialog::QueryMerchantDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryMerchantDialog)
{
    ui->setupUi(this);
    //设置窗口标题和窗口图标
    this->setWindowTitle("注册");
    this->setWindowIcon(QIcon(":/SuperMarketManageSystem/img/supermarket.png"));
    //设置对话框背景
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/loginbk1.jpg").scaled(this->size())));
    this->setPalette(palette);
    ui->lineEditMerchantCode->setPlaceholderText("请输入要查询的商品的完整或部分条码");
    ui->lineEditMerchantID->setPlaceholderText("请输入要查询的商品的编号");
    ui->lineEditMerchantName->setPlaceholderText("请输入要查询的商品的名称");

    //设置表头固定
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    this->setMaximumSize(542,284);
    this->setMaximumSize(542,284);
    connect(ui->pushButton,ui->pushButton->clicked,this,lookUp);
    connect(ui->pushButton_2,ui->pushButton_2->clicked,this,accept);
}

QueryMerchantDialog::~QueryMerchantDialog()
{
    delete ui;
}

void QueryMerchantDialog::lookUp()
{
    QString sID = ui->lineEditMerchantID->text();
    QString sCode = ui->lineEditMerchantCode->text();
    QString sName = ui->lineEditMerchantName->text();
    bool first = true;
    QString sql = "select merchantNumber,merchantCode,merchantName from merchant where ";
    if(sID!=""){
        first = false;
        sql = sql + " merchantNumber like '%" + sID + "%' ";
    }
    if(sCode!=""){
        if(!first){
            sql = sql + " and ";
        }
        sql = sql + " merchantCode like '%" + sCode + "%' ";
        first = false;
    }
    if(sName!=""){
        if(!first){
            sql = sql + " and ";
        }
        sql = sql + " merchantName like '%" + sName + "%' ";
        first = false;
    }
    if(first)sql = sql + " 1 ";
    QSqlQuery query(sql);
    if(!query.exec()){
        tips("错误","数据库错误");
        return;
    }
    int row = query.size(),k=0;
    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setColumnCount(3);
    while(query.next()){
        for(int i=0;i<3;++i){
            ui->tableWidget->setItem(k,i,new QTableWidgetItem(query.value(i).toString()));
        }
        ++k;
    }
}
