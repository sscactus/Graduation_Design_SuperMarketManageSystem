#include "loginwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    //设置标题、图标
    this->setWindowTitle("登录");
    this->setWindowIcon(QIcon(":/SuperMarketManageSystem/img/supermarket.png"));
    //设置窗口大小
    this->setMaximumSize(399,252);
    this->setMinimumSize(399,252);
    //设置标题颜色
    QPalette palette1;
    palette1.setColor(QPalette::WindowText,Qt::black);
    ui->labelTiltle->setPalette(palette1);
    //设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/loginbk1.jpg").scaled(this->size())));
    this->setPalette(palette);
    //设置三个按钮的图标
    ui->pushButtonLogin->setIcon(QIcon(":/SuperMarketManageSystem/img/Enter.png"));
    //ui->pushButtonLogin->setFlat(true);
    ui->pushButtonReg->setIcon(QIcon(":/SuperMarketManageSystem/img/log.png"));
    ui->pushButtonExit->setIcon(QIcon(":/SuperMarketManageSystem/img/exit.png"));
    //设置groupBox，放置两个radioButton
    type = new QButtonGroup(this);
    type->addButton(ui->radioButtonAdmin,0);
    type->addButton(ui->radioButtonSaler,1);
    //默认为营业员
    ui->radioButtonSaler->setChecked(true);
    //设置图片
    ui->labelPic->setPixmap(QPixmap(":/SuperMarketManageSystem/img/saler.png").scaled(ui->labelPic->size()));
    //设置登录按钮不可用
    ui->pushButtonLogin->setEnabled(false);
    //设置登录提示语
    ui->lineEditAccount->setPlaceholderText("请输入用户名");
    ui->lineEditPwd->setPlaceholderText("请输入密码");
    //设置密码输入显示为特殊字符
    ui->lineEditPwd->setEchoMode(QLineEdit::Password);
    //绑定信号和槽
    connect(ui->pushButtonLogin,ui->pushButtonLogin->clicked,this,this->loginBtnSlot);
    connect(ui->pushButtonReg,ui->pushButtonReg->clicked,this,this->regBtnSlot);
    connect(ui->pushButtonExit,ui->pushButtonExit->clicked,this,this->exitBtnSlot);
    connect(ui->lineEditAccount,ui->lineEditAccount->textChanged,this,this->loginBtnSetSlot);
    connect(ui->lineEditPwd,ui->lineEditPwd->textChanged,this,this->loginBtnSetSlot);
    connect(ui->radioButtonAdmin,ui->radioButtonAdmin->clicked,this,this->changePicToAdmin);
    connect(ui->radioButtonSaler,ui->radioButtonSaler->clicked,this,this->changePicToSaler);
}

loginWindow::~loginWindow()
{
    delete ui;
}
void loginWindow::clear()
{
    ui->lineEditAccount->clear();
    ui->lineEditPwd->clear();
}

void loginWindow::loginBtnSlot()
{
    QString userName = ui->lineEditAccount->text(),\
            passWord = ui->lineEditPwd->text(),workNumber;
    //对密码进行MD5编码
    QByteArray pwd = QCryptographicHash::hash(passWord.toUtf8(),QCryptographicHash::Md5);
    QString encodePassWord(pwd.toHex());
    passWord = encodePassWord;
    model = new QSqlTableModel(this);
    //Admin
    if(type->checkedId()==0){
        model->setTable("usrAdmin");
        model->select();
        for(int i=0;i<model->rowCount();++i){
            QSqlRecord record = model->record(i);
            if(record.value("accountName") == userName
                && record.value("passWord") == passWord){
                //登陆成功
                workNumber = record.value("workNumber").toString();
                QMessageBox::information(this,"登陆成功","欢迎您，尊敬的"+userName,QMessageBox::Ok);
                MainWindow *w = new MainWindow(0);
                this->hide();
                this->clear();
                w->setUserName(userName,workNumber);
                w->showMaximized();
                connect(w,w->toLoginWindow,this,showNormal);
                //emit showInfo(userName);
                return;
            }
            else if(record.value("accountName") == userName
                    && record.value("passWord") != passWord){
                QMessageBox::information(this,"错误","密码或账号输入错误，请重新输入",
                                         QMessageBox::Yes);
                this->clear();
                return;
            }
        }
        QMessageBox msg;
        msg.setText("此用户不存在！！");
        msg.setInformativeText("请问您要注册么");
        QPushButton *reg = msg.addButton("注册",QMessageBox::ActionRole);
        msg.addButton(QMessageBox::Cancel);
        msg.exec();
        if(msg.clickedButton() == reg){
            //注册
            regBtnSlot();
        }
    }
    //Saler
    else if(type->checkedId()==1){
        model->setTable("usrSaler");
        model->select();
        for(int i=0;i<model->rowCount();++i){
            QSqlRecord record = model->record(i);
            if(record.value("accountName") == userName
                && record.value("passWord") == passWord){
                //登陆成功
                workNumber = record.value("workNumber").toString();
                QMessageBox::information(this,"登陆成功","欢迎您，尊敬的"+userName,QMessageBox::Ok);
                MainWindow_sale *w = new MainWindow_sale(0);
                this->hide();this->clear();
                w->setUserName(userName,workNumber);
                w->showMaximized();
                connect(w,w->toLoginWindow,this,showNormal);
                //emit showInfo(userName);
                return;
            }
            else if(record.value("accountName") == userName
                    && record.value("passWord") != passWord){
                QMessageBox::information(this,"错误","密码或账号输入错误，请重新输入",
                                         QMessageBox::Yes);
                this->clear();
                return;
            }
        }
        QMessageBox msg;
        msg.setText("此用户不存在！！");
        msg.setInformativeText("请问您要注册么");
        QPushButton *reg = msg.addButton("注册",QMessageBox::ActionRole);
        msg.addButton(QMessageBox::Cancel);
        msg.exec();
        if(msg.clickedButton() == reg){
            //注册，直接调用regBtnSlot
            regBtnSlot();
        }
    }
}

void loginWindow::regBtnSlot()
{
    this->hide();
    registerWindow *regWindow = new registerWindow(0);
    regWindow->setWindowFlags(Qt::WindowMinimizeButtonHint);
    connect(regWindow,regWindow->toLoginDialog,this,[=](){
        showNormal();
        regWindow->close();
        clear();
        delete regWindow;
    });
    regWindow->show();
}

void loginWindow::exitBtnSlot()
{
    this->close();
}

void loginWindow::loginBtnSetSlot(QString)
{
    if(ui->lineEditAccount->text()!=""&&ui->lineEditPwd->text()!="")
        ui->pushButtonLogin->setEnabled(true);
    else
        ui->pushButtonLogin->setEnabled(false);
}

void loginWindow::changePicToAdmin()
{
    ui->labelPic->setPixmap(QPixmap(":/SuperMarketManageSystem/img/admin.png").scaled(ui->labelPic->size()));
}

void loginWindow::changePicToSaler()
{
    ui->labelPic->setPixmap(QPixmap(":/SuperMarketManageSystem/img/saler.png").scaled(ui->labelPic->size()));
}
