#include "registerwindow.h"
#include "ui_registerwindow.h"

registerWindow::registerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerWindow)
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
    ui->labelStar9->setPalette(palette2);
    //添加GroupButton组合RadioButton
    type = new QButtonGroup(this);
    type->addButton(ui->radioButtonAdmin,0);
    type->addButton(ui->radioButtonSaler,1);
    //设置默认选择为营业员
    ui->radioButtonSaler->setChecked(true);
    //隐藏注册码
    ui->labelAdminNumber->setVisible(false);
    ui->lineEditAdminNumber->setVisible(false);
    ui->labelStar9->setVisible(false);
    //设置三个按钮的图标
    ui->pushButtonReg->setIcon(QPixmap(":/SuperMarketManageSystem/img/reg.png"));
    ui->pushButtonReturn->setIcon(QPixmap(":/SuperMarketManageSystem/img/return.png"));
    ui->pushButtonClear->setIcon(QPixmap(":/SuperMarketManageSystem/img/reset.png"));
    //设置注册按钮不可用
    ui->pushButtonReg->setEnabled(false);
    //设置密码显示模式为隐藏
    ui->lineEditConfirm->setEchoMode(QLineEdit::Password);
    ui->lineEditPwd->setEchoMode(QLineEdit::Password);
    //绑定按钮和槽
    connect(ui->pushButtonReg,ui->pushButtonReg->clicked,this,this->registerAccount);
    connect(ui->pushButtonClear,ui->pushButtonClear->clicked,this,this->clear);
    connect(ui->pushButtonReturn,ui->pushButtonReturn->clicked,this,this->backToLogin);
    connect(ui->lineEditAccount,ui->lineEditAccount->textChanged,this,this->enableReg);
    connect(ui->lineEditPwd,ui->lineEditPwd->textChanged,this,this->enableReg);
    connect(ui->lineEditConfirm,ui->lineEditConfirm->textChanged,this,this->enableReg);
    connect(ui->radioButtonAdmin,ui->radioButtonAdmin->clicked,this,this->setAdminNumber);
    connect(ui->radioButtonSaler,ui->radioButtonSaler->clicked,this,this->hideAdminNumber);
}

registerWindow::~registerWindow()
{
    delete ui;
}

void registerWindow::setTips()
{
    ui->lineEditAccount->setPlaceholderText("请输入账号");
    ui->lineEditAddress->setPlaceholderText("请输入地址");
    ui->lineEditConfirm->setPlaceholderText("请再次输入密码");
    ui->lineEditMail->setPlaceholderText("请输入邮件地址,如123@abc.cn");
    ui->lineEditName->setPlaceholderText("请输入姓名");
    ui->lineEditPhone->setPlaceholderText("请输入电话号码");
    ui->lineEditPwd->setPlaceholderText("请输入密码");
    ui->lineEditQQ->setPlaceholderText("请输入qq号码");
    ui->lineEditAdminNumber->setPlaceholderText("请输入注册码，没有请咨询管理员！");
}

void registerWindow::registerAccount()
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
    model = new QSqlTableModel(this);
    if(type->checkedId() == 0)model->setTable("usrAdmin");
    else if(type->checkedId() == 1)model->setTable("usrSaler");
    model->select();
    for(int i=0;i<model->rowCount();++i){
        QSqlRecord record = model->record(i);
        if(record.value("accountName") == accountName){
            QMessageBox::information(this,"提示","账号已存在，请重新输入账号！",QMessageBox::Ok);
            clear();
            return;
        }
    }
    //对数据格式进行检查，邮件
    //账号不存在,可以注册
    //用MD5对密码进行加密
    QByteArray pwd = QCryptographicHash::hash(passWord1.toUtf8(),QCryptographicHash::Md5);
    QString encodePassWord(pwd.toHex());
    passWord1 = encodePassWord;

    //管理员
    if(type->checkedId() == 0){
        int t = ui->lineEditAdminNumber->text().toInt();
        QString str = QString::number(t-1,10);
        qDebug() << str << " " << t;
        QByteArray adminNumber = QCryptographicHash::hash(str.toUtf8(),QCryptographicHash::Md5);
        QString encodePassWord(adminNumber.toHex());
        QString adm = encodePassWord;
        //验证注册码
        model->setTable("usrAdmin");
        model->select();
        QSqlRecord record = model->record(model->rowCount() - 1);
        qDebug() << record.value("adminNumber") << " " << adm;
        if(record.value("adminNumber") != adm){
            QMessageBox::information(this,"错误","你输入的注册码有误，请联系管理员！",QMessageBox::Ok);
            return;
        }
        str = QString(t);
        adminNumber = QCryptographicHash::hash(str.toUtf8(),QCryptographicHash::Md5);
        QString tmp(adminNumber.toHex());
        adm = tmp;
        //写入数据库
        QSqlQuery query;
        query.exec("select * from usrinfo");
        int workNumber = query.size() + 1;
        //写入usradmin表
        query.prepare("INSERT INTO usrAdmin (accountName, passWord,adminNumber,workNumber) "
                      "VALUES (:accountName, :passWord, :adminNumber,:workNumber)");
        query.bindValue(":accountName", accountName);
        query.bindValue(":passWord", passWord1);
        query.bindValue(":adminNumber",adm);
        query.bindValue(":workNumber",workNumber);
        if(!query.exec()){
            QMessageBox::information(this,"错误",
                                     "数据库出错,"+query.lastError().text()+",请稍后重试",QMessageBox::Ok);
            return;
        }

    }
    //营业员
    else if(type->checkedId() == 1){
        //写入数据库
        QSqlQuery query;
        query.exec("select * from usrinfo");
        int workNumber = query.size() + 1;
        //写入usrsaler表
        query.prepare("INSERT INTO usrSaler (accountName, passWord, workNumber) "
                      "VALUES (:accountName, :passWord, :workNumber)");
        query.bindValue(":accountName", accountName);
        query.bindValue(":passWord", passWord1);
        query.bindValue(":workNumber", workNumber);
        if(!query.exec()){
            QMessageBox::information(this,"错误",
                                     "数据库出错,"+query.lastError().text()+",请稍后重试",QMessageBox::Ok);
            return;
        }
    }
    //写入usrinfo表
    QSqlQuery query;
    QString insert = "INSERT INTO usrinfo (accountName, name, telephone, qq, mail, address)"
                     "VALUES ('" + accountName + "', '" + ui->lineEditName->text() + "', '" + ui->lineEditPhone->text()
                      + "', '" + ui->lineEditQQ->text() + "', '" + ui->lineEditMail->text() + "', '" + ui->lineEditAddress->text()
                      + "')";
    if(!query.exec(insert)){
        //前面插入成功，后面失败，则要删除前面插入的
        query.exec("DELETE FROM usradmin WHERE accountName = '" + accountName + "'");
        QMessageBox::information(this,"错误",
                                 "数据库出错,"+query.lastError().text()+\
                                 ",请稍后重试",QMessageBox::Ok);
        return;
    }
    //写入成功
    QPixmap picture(":/SuperMarketManageSystem/img/m10.png");
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    picture.save(&buffer,"PNG");
    QString out;
    for(int k = 0; k < (int)bytes.size(); ++k)
    {
        QString str;
        str.sprintf("%02x",(uchar)bytes[k]);
        out.append(str);
    }
    query.exec(QString("update usrinfo set photo = 0x%1 where accountname = %2")\
               .arg(out).arg(accountName));
    //成功注册
    QMessageBox::information(this,"注册成功","恭喜您，注册成功！",QMessageBox::Ok);
    this->accept();
}

void registerWindow::clear()
{
    ui->lineEditAccount->clear();
    ui->lineEditAddress->clear();
    ui->lineEditConfirm->clear();
    ui->lineEditMail->clear();
    ui->lineEditName->clear();
    ui->lineEditPhone->clear();
    ui->lineEditPwd->clear();
    ui->lineEditQQ->clear();
    ui->lineEditAdminNumber->clear();
}

void registerWindow::backToLogin()
{
    emit toLoginDialog();
}

void registerWindow::enableReg(QString)
{
    if(ui->lineEditAccount->text() != ""
       &&ui->lineEditConfirm->text() != ""
       &&ui->lineEditPwd->text() != "")
        ui->pushButtonReg->setEnabled(true);
    else ui->pushButtonReg->setEnabled(false);
}

void registerWindow::setAdminNumber()
{
    clear();
    ui->labelStar9->setVisible(true);
    ui->labelAdminNumber->setVisible(true);
    ui->lineEditAdminNumber->setVisible(true);
}

void registerWindow::hideAdminNumber()
{
    clear();
    ui->labelStar9->setVisible(false);
    ui->labelAdminNumber->setVisible(false);
    ui->lineEditAdminNumber->setVisible(false);
}
