#include "outlibdialog.h"
#include "ui_outlibdialog.h"
#include "global.h"

outLibDialog::outLibDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outLibDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("出库");
    this->setWindowIcon(QIcon(":/SuperMarketManageSystem/img/supermarket.png"));
    //设置对话框大小
    this->setMaximumSize(307,284);
    this->setMinimumSize(307,284);
    //设置对话框背景
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/loginbk1.jpg").scaled(this->size())));
    this->setPalette(palette);
    QSqlQuery query("select storeNumber from store");
    if(!query.exec()){
        tips("错误","数据库出错");
        return;
    }
    ui->spinBox->setMinimum(1);
    ui->comboBoxStore->clear();
    while(query.next()){
        ui->comboBoxStore->addItem(query.value("storeNumber").toString());
    }
    QString sql = "select merchantNumber from merchantStore where storeNumber = '"+ui->comboBoxStore->currentText()+"'";
    ui->comboBoxMerchant->clear();
    if(!query.exec(sql)){
        tips("错误","数据库出错");
        return;
    }
    while(query.next()){
        ui->comboBoxMerchant->addItem(query.value("merchantNumber").toString());
    }
    connect(ui->comboBoxStore,ui->comboBoxStore->currentTextChanged,[=](QString sID){
        QSqlQuery query;
        QString sql = "select merchantNumber from merchantStore where storeNumber = '"+sID+"'";
        ui->comboBoxMerchant->clear();
        if(!query.exec(sql)){
            tips("错误","数据库出错");
            return;
        }
        while(query.next()){
            ui->comboBoxMerchant->addItem(query.value("merchantNumber").toString());
        }
    });
    connect(ui->comboBoxMerchant,ui->comboBoxMerchant->currentTextChanged,[=](QString sID){
        QSqlQuery query;
        QString sql = "select number from merchantStore where merchantNumber = '"+sID+"' and storeNumber = '"+ui->comboBoxStore->currentText()+"'";
        if(!query.exec(sql)){
            tips("错误","数据库出错");
            return;
        }
        if(query.next())ui->spinBox->setMaximum(query.value("number").toInt());
    });
    connect(ui->pushButtonYes,ui->pushButtonYes->clicked,this,outLib);
    connect(ui->pushButtonNo,ui->pushButtonNo->clicked,[=](bool){
        tips("提示","取消出库");
        this->reject();
    });
}

outLibDialog::~outLibDialog()
{
    delete ui;
}

void outLibDialog::outLib()
{
    QString merchantID = ui->comboBoxMerchant->currentText();
    QString storeID = ui->comboBoxStore->currentText();
    QString sql;
    QString num = ui->spinBox->text();
    QSqlQuery query;
    //更新merchantStore
    if(num.toInt() == ui->spinBox->maximum()){
        sql = "delete from merchantStore where storeNumber = '"+storeID+"' and merchantNumber = '"+merchantID+"';";
    }else{
        sql = "update merchantStore set number = number - "+num+" where storeNumber = '"+storeID+"' and merchantNumber = '"+merchantID+"';";
    }
    //更新store
    sql = sql + "update store set leftCap = leftCap + "+num+" where storeNumber = '"+storeID+"';";
    //更新goodsShelve
    if(!query.exec("select * from goodsshelve where merchantNumber = '"+merchantID+"'")){
        tips("错误","数据库出错");
        return;
    }

    if(query.size()){
        sql = sql + "update goodsShelve set number = number +"+num+" where merchantNumber = '"+merchantID+"';";
    }
    else{
        sql = sql + "insert into goodsShelve values('"+merchantID+"','"+num+"');";
    }
    if(!query.exec(sql)){
        tips("错误","数据库出错");
        return;
    }
    tips("提示","出库成功");
    this->accept();
}

