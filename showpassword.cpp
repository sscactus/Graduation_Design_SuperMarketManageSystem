#include "showpassword.h"
#include "ui_showpassword.h"
#include "global.h"

ShowPassWord::ShowPassWord(QString ID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPassWord)
{
    ui->setupUi(this);

    this->setWindowTitle("请输入密码");
    this->setWindowIcon(QIcon(":/SuperMarketManageSystem/img/supermarket.png"));
    ui->lineEditPwd->setEchoMode(QLineEdit::Password);
    //设置对话框背景
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/loginbk1.jpg").scaled(this->size())));
    this->setPalette(palette);
    connect(ui->pushButtonYes,ui->pushButtonYes->clicked,this,[=](bool){
        QString pwd = ui->lineEditPwd->text();
        QSqlQuery query("SELECT password from viptable where VIPNumber = '"+ID+"'");
        if(!query.exec()){
            tips("错误","数据库错误");
            return;
        }
        if(query.next()&&query.value("passWord").toString() == getMd5Pwd(pwd)){
            this->accept();
        }
        else{
            this->reject();
        }
    });
    connect(ui->pushButtonCancel,ui->pushButtonCancel->clicked,this,[=](bool){
        this->reject();
    });
}

ShowPassWord::~ShowPassWord()
{
    delete ui;
}
