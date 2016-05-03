#include "memberdialog.h"
#include "ui_memberdialog.h"
#include "global.h"

memberDialog::memberDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::memberDialog)
{
    ui->setupUi(this);
    //设置窗口标题和窗口图标
    this->setWindowTitle("注册");
    this->setWindowIcon(QIcon(":/SuperMarketManageSystem/img/supermarket.png"));
    //设置对话框大小
    this->setMaximumSize(399,360);
    this->setMinimumSize(399,360);
    //设置对话框背景
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/loginbk1.jpg").scaled(this->size())));
    this->setPalette(palette);
    //设置对话框标题颜色
    QPalette palette1;
    palette1.setColor(QPalette::WindowText,Qt::black);
    ui->labelTitle->setPalette(palette1);
    //设置提示语
    setTips();
    //必填项加红色星号
    QPalette palette2;
    palette2.setColor(QPalette::WindowText,Qt::red);
    ui->labelStar1->setPalette(palette2);
    ui->labelStar2->setPalette(palette2);
    ui->labelStar3->setPalette(palette2);
    ui->labelStar4->setText(" ");
    ui->labelStar5->setText(" ");
    ui->labelStar6->setText(" ");
    ui->labelStar7->setText(" ");
    ui->labelStar8->setText(" ");
//    ui->labelStar9->setPalette(palette2);
//    //添加GroupButton组合RadioButton
//    type = new QButtonGroup(this);
//    type->addButton(ui->radioButtonAdmin,0);
//    type->addButton(ui->radioButtonSaler,1);
//    //设置默认选择为营业员
//    ui->radioButtonSaler->setChecked(true);
//    //隐藏注册码
//    ui->labelAdminNumber->setVisible(false);
//    ui->lineEditAdminNumber->setVisible(false);
//    ui->labelStar9->setVisible(false);
    //设置三个按钮的图标
    ui->pushButtonReg->setIcon(QPixmap(":/SuperMarketManageSystem/img/reg.png"));
    ui->pushButtonReturn->setIcon(QPixmap(":/SuperMarketManageSystem/img/return.png"));
    ui->pushButtonClear->setIcon(QPixmap(":/SuperMarketManageSystem/img/reset.png"));
    //设置注册按钮不可用
    ui->pushButtonReg->setEnabled(false);
    //设置密码显示模式为隐藏
    ui->lineEditConfirm->setEchoMode(QLineEdit::Password);
    ui->lineEditPwd->setEchoMode(QLineEdit::Password);

    connect(ui->pushButtonReg,ui->pushButtonReg->clicked,this,registerAccount);
    connect(ui->pushButtonClear,ui->pushButtonClear->clicked,this,clear);
    connect(ui->pushButtonReturn,ui->pushButtonReturn->clicked,[=](bool){
        this->accept();
    });
    connect(ui->lineEditAccount,ui->lineEditAccount->textChanged,this,this->enableReg);
    connect(ui->lineEditPwd,ui->lineEditPwd->textChanged,this,this->enableReg);
    connect(ui->lineEditConfirm,ui->lineEditConfirm->textChanged,this,this->enableReg);
}

memberDialog::~memberDialog()
{
    delete ui;
}

void memberDialog::setTips()
{
    ui->lineEditAccount->setPlaceholderText("请输入账号");
    ui->lineEditAddress->setPlaceholderText("请输入地址");
    ui->lineEditConfirm->setPlaceholderText("请再次输入密码");
    ui->lineEditMail->setPlaceholderText("请输入邮件地址,如123@abc.cn");
    ui->lineEditName->setPlaceholderText("请输入姓名");
    ui->lineEditPhone->setPlaceholderText("请输入电话号码");
    ui->lineEditPwd->setPlaceholderText("请输入密码");
    ui->lineEditQQ->setPlaceholderText("请输入qq号码");
}

void memberDialog::registerAccount()
{
    //获取账号，密码
    QString accountName = ui->lineEditAccount->text();
    QString passWord1 = ui->lineEditPwd->text();
    QString passWord2 = ui->lineEditConfirm->text();
    //账号，密码为空时不允许注册
    //判断两次密码是否一致
    if(passWord1 != passWord2){
        QMessageBox::information(this,"提示","两次密码不一致，请重新输入！",QMessageBox::Ok);
        clear();
        return;
    }
    //密码一致，查询数据库，检查该账号是否已存在
    QSqlTableModel *model = new QSqlTableModel(this);
    model = new QSqlTableModel(this);
    model->setTable("VIPTable");
    model->select();
    for(int i=0;i<model->rowCount();++i){
        QSqlRecord record = model->record(i);
        if(record.value("VIPNumber") == accountName){
            QMessageBox::information(this,"提示","账号已存在，请重新输入账号！",QMessageBox::Ok);
            clear();
            return;
        }
    }
    //对数据格式进行检查，邮件
    if(ui->lineEditMail->text()!=""){
        if(!isLegalMail(ui->lineEditMail->text())){
            tips("提示","请输入合法的邮件地址");
            return;
        }
    }
    //账号不存在,可以注册
    //用MD5对密码进行加密
//    QByteArray pwd = QCryptographicHash::hash(passWord1.toUtf8(),QCryptographicHash::Md5);
//    QString encodePassWord(pwd.toHex());
    passWord1 = getMd5Pwd(passWord2);
    QSqlQuery query("insert into viptable values('"+accountName+"','"+ui->lineEditName->text()+\
                    "','100','"+ui->lineEditMoney->text()+"','0','"+ui->lineEditPhone->text()+"','"+\
                    ui->lineEditQQ->text()+"','"+ui->lineEditMail->text()+"','"+\
                    ui->lineEditAddress->text()+"','"+passWord1+"')");
    this->accept();
}

void memberDialog::clear()
{
    ui->lineEditAccount->clear();
    ui->lineEditAddress->clear();
    ui->lineEditConfirm->clear();
    ui->lineEditMail->clear();
    ui->lineEditName->clear();
    ui->lineEditPhone->clear();
    ui->lineEditPwd->clear();
    ui->lineEditQQ->clear();
}

void memberDialog::enableReg(QString)
{
    if(ui->lineEditAccount->text() != ""
       &&ui->lineEditConfirm->text() != ""
       &&ui->lineEditPwd->text() != "")
        ui->pushButtonReg->setEnabled(true);
    else ui->pushButtonReg->setEnabled(false);
}
