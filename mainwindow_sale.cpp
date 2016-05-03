#include "mainwindow_sale.h"
#include "ui_mainwindow_sale.h"
#include "datedelegate.h"
#include "comboxdelegat.h"
#include <QKeyEvent>
#include "showpassword.h"
#include <QFileDialog>
#include "memberdialog.h"
#include "querymerchantdialog.h"
#include <QMap>

MainWindow_sale::MainWindow_sale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_sale)
{
    ui->setupUi(this);
    setWindowTitle("超市信息管理系统");
    infoState = true;
    changeInfoShowState();

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->stackedWidgetSale->setCurrentIndex(0);
    ui->menuHelp->setShortcutEnabled(true);
    this->setObjectName("mainWindow");
    this->setStyleSheet("#mainWindow{border-image:url(:/SuperMarketManageSystem/img/loginbk1.jpg);}");
    //install eventfilter
    ui->menuShangPinXiaoShou->installEventFilter(this);
    //ui->menuZhangHuGuanLi->installEventFilter(this);
    ui->menuHelp->installEventFilter(this);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

//    cDateDelegate dateDelegate;
//    ui->tableWidget->setItemDelegateForColumn(7,&dateDelegate);
//    ComboxDelegat cComboBoxDelegate;
//    ui->tableWidget->setItemDelegateForColumn(5,&cComboBoxDelegate);

    initShow();
    price = accMoney = 0.00;
    isVIP = false;
    nCnt = 0;
    selectedRow = -1;

    connect(ui->tableWidget,ui->tableWidget->itemSelectionChanged,this,[=](){
        selectedRow = ui->tableWidget->currentRow();
    });
    connect(ui->actionZhuXiao,ui->actionZhuXiao->triggered,this,[=](){
        returnToLogin();
    });
    connect(ui->actionTuiChu,ui->actionTuiChu->triggered,this,[=](){
        exitSys();
    });
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,changeInfoShowState);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,savePrsonInfo);
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,clearPwd);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,updatePwd);
    connect(ui->actionGeRenXinXi,ui->actionGeRenXinXi->triggered,this,[=](){
        ui->stackedWidgetSale->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(0);
        ui->pushButtonChange->disconnect();
        ui->pushButtonSave->disconnect();
        connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,changeInfoShowState);
        connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,savePrsonInfo);

        ui->pushButtonChange->setText("修改");
        ui->pushButtonSave->setText("保存");
        if(infoState)changeInfoShowState();
    });
    connect(ui->actionMiMaXiuGai,ui->actionMiMaXiuGai->triggered,this,[=](){
        ui->stackedWidgetSale->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
        ui->pushButtonChange->disconnect();
        ui->pushButtonSave->disconnect();
        connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,clearPwd);
        connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,updatePwd);
        ui->pushButtonSave->setEnabled(true);
        ui->pushButtonChange->setText("重置");
        ui->pushButtonSave->setText("确定");
    });
    connect(ui->accountManageListWidget,ui->accountManageListWidget->currentRowChanged,this,[=](int row){
        switch(row){
        case 0:
            ui->stackedWidgetSale->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->pushButtonChange->disconnect();
            ui->pushButtonSave->disconnect();
            connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,changeInfoShowState);
            connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,savePrsonInfo);

            ui->pushButtonChange->setText("修改");
            ui->pushButtonSave->setText("保存");
            if(infoState)changeInfoShowState();
            break;
        case 1:

            ui->stackedWidgetSale->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(1);
            ui->pushButtonChange->disconnect();
            ui->pushButtonSave->disconnect();
            connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,clearPwd);
            connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,updatePwd);
            ui->pushButtonSave->setEnabled(true);
            ui->pushButtonChange->setText("重置");
            ui->pushButtonSave->setText("确定");
            break;
        case 2:
            returnToLogin();
            break;
        case 3:
            exitSys();
            break;
        }
    });
    connect(ui->pushButtonUpdate,ui->pushButtonUpdate->clicked,this,updatePhoto);
    connect(ui->spinBoxNum,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),\
            this,calcuMoney);
    connect(ui->lineEditBianHao,ui->lineEditBianHao->textChanged,this,getPriceFromID);
    connect(ui->lineEditTiaoMa,ui->lineEditTiaoMa->textChanged,this,getPriceFromCode);
    connect(ui->lineEditHuiYuanNumber,ui->lineEditHuiYuanNumber->textChanged,this,showVIPInfo);
    connect(ui->zengPingListWidget,ui->zengPingListWidget->currentRowChanged,this,[=](int r){
        ui->labelZPMC->setText(ui->zengPingListWidget->currentItem()->text());
        ui->labelScorReq->setText(QString("%1").arg((r+2)*4));
        ui->labelNumCanH->setText(QString("20"));
    });
}

MainWindow_sale::~MainWindow_sale()
{
    delete ui;
}

void MainWindow_sale::getPriceFromID(QString sID)
{
    QSqlQuery query;
    QString sql = "select * from merchant where merchantNumber = '"+sID+"'";
    if(ui->lineEditTiaoMa->text()!="")sql=sql+" and merchantcode = '"+ui->lineEditTiaoMa->text()+"'";

    if(!query.exec(sql))return;
    if(query.size()<=0){
        statusBar()->setToolTip("商品条码和编号不对应");
        ui->labelMoneySum->setText("0.00");
        return;
    }
    sql = "select salePrice from inMerchantOrder where MerchantNumber = '"+sID+"';";

    if(!query.exec(sql))return;
    if(query.next())price = query.value("salePrice").toString().toDouble();
    double money = price * ui->spinBoxNum->text().toDouble();
    ui->labelMoneySum->setText(sql.sprintf("%.2lf",money));
}

void MainWindow_sale::getPriceFromCode(QString sCode)
{
    QSqlQuery query;
    if(ui->lineEditBianHao->text()!=""){
        getPriceFromID(ui->lineEditBianHao->text());
        return;
    }
    QString sql = "select * from merchant where merchantCode = '"+sCode+"'";
    if(ui->lineEditBianHao->text()!=""){
        sql=sql+" and merchantNumber = '"+ui->lineEditBianHao->text()+"'";
    }
    if(!query.exec(sql)){
        return;
    }
    if(query.size()<=0){
        statusBar()->setToolTip("商品条码和编号不对应");
        ui->labelMoneySum->setText("0.00");
        return;
    }
    QString sID = query.value("MerchantNumber").toString();
    sql = "select salePrice form inMerchantOrder where merchantNumber = '"+sID+"'";

    if(!query.exec(sql))return;
    if(query.next())price = query.value("salePrice").toString().toDouble();
    double money = price * ui->spinBoxNum->text().toDouble();

    ui->labelMoneySum->setText(sql.sprintf("%.2lf",money));
}

void MainWindow_sale::showVIPInfo(QString sID)
{
    QSqlQuery query;
    QString sql = "select * from vipTable where VIPNumber = '"+sID+"';";
    if(!query.exec(sql))return;
    if(query.size()==0){
        isVIP = false;
        for(int i=0;i<ui->tableWidget->rowCount();++i){
            ui->tableWidget->item(i,4)->setText("0");
        }
        ui->labelHuiYuanMingCheng_2->setText("");
        ui->labelHuiYuanJiFen_2->setText("0");
        ui->labelHuiYuanJinE_2->setText("0");
        ui->zengPingListWidget->clear();
    }
    else if(query.next()){
        isVIP = true;
        ui->labelHuiYuanMingCheng_2->setText(query.value("VIPNick").toString());
        ui->labelHuiYuanJiFen_2->setText(query.value("score").toString());//积分
        ui->labelHuiYuanJinE_2->setText(query.value("money").toString());//卡内剩余金额
        for(int i=0;i<ui->tableWidget->rowCount();++i){
            ui->tableWidget->item(i,4)->setText("0.2");

        }
        if(!query.exec("select * from merchant")){
            tips("出错","数据库错误");
            return;
        }
        int k = 0;
        while(query.next()){
            if(k==10)break;
            ui->zengPingListWidget->addItem(query.value("merchantname").toString());
            ++k;
        }
    }
}

void MainWindow_sale::calcuMoney(int nNum)
{
    //        QString sSql,sID,sCode;
    //        bool first = true;
    //        sID = ui->lineEditBianHao->text();
    //        sCode = ui->lineEditTiaoMa->text();
    //        sSql = "select * from inmerchantorder ";
    //        if(sID!=""){
    //            first = false;
    //            sSql = sSql + " where merchantNumber = '"+sID+"'";
    //        }
    //        if(SCode!=""){
    //            if(!first){
    //                sSql += " and ";
    //            }else{
    //                sSql = sSql+" where ";
    //                first = false;
    //            }
    //            sSql = sSql+" merchantCode = '"+sCode+"'";
    //        }
    //        if(first)return;
    //        QSqlQuery query;
    //        if(!query.exec(sSql)){
    //            tips("错误","数据库错误");
    //        }

    double res = price * double(nNum);
    QString s;
    s.sprintf("%.2lf",res);
    ui->labelMoneySum->setText(s);
}

void MainWindow_sale::initShow()
{
    QSqlQuery query;
    this->setFocusPolicy(Qt::StrongFocus);
    //设置焦点在商品编号上
    ui->labelBianHao->setFocus();
    if(!query.exec("select * from saleMerchantOrder")){
        tips("错误","数据库错误!");
        return;
    }
    int q = query.size() + 1;
    QString t,d=QDate::currentDate().toString("yyyyMMdd");
    t.sprintf("%06d",q);
    QString orderID = QString("%1%2").arg(d).arg(t);
    ui->labelLiuShuiNumber1->setText(orderID);
    ui->labelOperate1->setText(QHostInfo::localHostName());
//    int w = ui->frameShangPin->frameWidth();
////    ui->tableWidget->setColumnWidth(0,36);
////    w -= 36;
//    for(int i=0;i<ui->tableWidget->columnCount();++i){
//        ui->tableWidget->setColumnWidth(i,w/8);
//    }
    //设置提示信息为红色
    QPalette palette1;
    palette1.setColor(QPalette::WindowText,Qt::red);
    ui->frameHelp->setPalette(palette1);
    //
    ui->labelMerchantNumber_2->setText("0");
    ui->labelMoney_2->setText("0.00");
    ui->labelMoneySum->setText("0.00");
    ui->labelHuiYuanJiFen_2->setText("0");
    ui->labelHuiYuanJinE_2->setText("0");
    ui->labelXiaoFeiXiaoFeiJinE_2->setText("0.00");
    ui->labelHuiYuanMingCheng_2->setText("");
    ui->labelScorReq->setText("0");
    ui->labelNumCanH->setText("0");
    ui->labelZPMC->setText("    ");
}

void MainWindow_sale::setUserName(QString userName, QString workNumber)
{
    this->userName = userName;
    this->wkNumber = workNumber;
    setUpPersonInfo(userName,wkNumber);
}

void MainWindow_sale::setUpPersonInfo(QString Id, QString wkNum)
{
    QString sql = "select * from usrinfo where accountName = '" + Id +"' and worknumber = '"\
            +wkNum+"';";
    QSqlQuery query(sql);
    if(!query.exec()){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
        return ;
    }
#ifdef DEBUG_LOG
    QMessageBox::information(this,"tips","in the setupPersonInfo",QMessageBox::Ok);
#endif
    if(query.next()) {
        ui->accountLineEdit_2->setText(query.value("accountName").toString());
        ui->nameLineEdit_2->setText(query.value("name").toString());
        if(query.value("sex").toString()=="男"){
            ui->sexComboBox_2->setCurrentIndex(0);
        }else if(query.value("sex").toString()=="女"){
            ui->sexComboBox_2->setCurrentIndex(1);
        }
        else{
            ui->sexComboBox_2->setCurrentIndex(2);
        }
        ui->deparTextEdit_2->setText(query.value("Department").toString());
        ui->ageLineEdit_2->setText(query.value("age").toString());
        ui->otherLabel_2->setText("备注："+query.value("remark").toString());
        QPixmap photo;
        if(!query.isNull("photo"))photo.loadFromData(query.value("photo").toByteArray(),"PNG");
        ui->labelPic->setPixmap(photo);
        ui->textEditPersonIntroduce->setText(query.value("personIntroduce").toString());
        ui->lineEditMail->setText(query.value("mail").toString());
        ui->lineEditAddress->setText(query.value("address").toString());
        ui->lineEditPostCode->setText(query.value("postcode").toString());
        ui->lineEditTelephone->setText(query.value("telephone").toString());
        ui->lineEditFixedphone->setText(query.value("fixPhone").toString());
        ui->lineEditCountry->setText(query.value("country").toString());
        ui->lineEditProvince->setText(query.value("province").toString());
        ui->lineEditCity->setText(query.value("city").toString());
        ui->lineEditDetailAddress->setText(query.value("detailAddress").toString());
        ui->textEditPeronDetail->setText(query.value("detailPersonInfo").toString());
        if(query.value("isReceiveMsg").toInt() == 1){
            ui->checkBoxIsR->setChecked(true);
        }else{
            ui->checkBoxIsR->setChecked(false);
        }
    }
}
void MainWindow_sale::changeInfoShowState()
{
    infoState = !infoState;
    if(infoState)ui->pushButtonChange->setText("取消");
    else{
        ui->pushButtonChange->setText("修改");
        setUpPersonInfo(userName,wkNumber);
    }
    ui->pushButtonSave->setEnabled(infoState);
    ui->nameLineEdit_2->setEnabled(infoState);
    ui->accountLineEdit_2->setEnabled(false);
    ui->ageLineEdit_2->setEnabled(infoState);
    ui->sexComboBox_2->setEnabled(infoState);
    ui->deparTextEdit_2->setEnabled(infoState);
    ui->textEditPersonIntroduce->setEnabled(infoState);
    ui->lineEditAddress->setEnabled(infoState);
    ui->lineEditCity->setEnabled(infoState);
    ui->lineEditCountry->setEnabled(infoState);
    ui->lineEditProvince->setEnabled(infoState);
    ui->lineEditDetailAddress->setEnabled(infoState);
    ui->lineEditFixedphone->setEnabled(infoState);
    ui->lineEditMail->setEnabled(infoState);
    ui->lineEditPostCode->setEnabled(infoState);
    ui->lineEditTelephone->setEnabled(infoState);
    ui->textEditPeronDetail->setEnabled(infoState);
    ui->checkBoxIsR->setEnabled(infoState);
}

void MainWindow_sale::savePrsonInfo()
{
    QString name = ui->nameLineEdit_2->text();
    QString sex = ui->sexComboBox_2->currentText();
    QString depar = ui->deparTextEdit_2->toPlainText();
    QString age = ui->ageLineEdit_2->text();
    QString personIntroduce = ui->textEditPersonIntroduce->toPlainText();
    QString mail = ui->lineEditMail->text();
    QString address = ui->lineEditAddress->text();
    QString postCode = ui->lineEditPostCode->text();
    QString telePhone = ui->lineEditTelephone->text();
    QString officePhone = ui->lineEditFixedphone->text();
    QString isReceiveMsg;
    if(ui->checkBoxIsR->isChecked()){
        isReceiveMsg = "1";
    }else{
        isReceiveMsg = "0";
    }
    QString country = ui->lineEditCountry->text();
    QString province = ui->lineEditProvince->text();
    QString city = ui->lineEditCity->text();
    QString detailAddress = ui->lineEditDetailAddress->text();
    QString detailPersonInfo = ui->textEditPeronDetail->toPlainText();
    QString sql="update usrinfo set";
    bool first = true;
    if(name!=""){
        if(!first){
            sql=sql+",";
        }
        sql=sql+" name = '" + name + "'";
        first = false;
    }
    if(depar!=""){
        if(!first){
            sql=sql+",";
        }
        sql=sql+" Department = '" + depar + "'";
        first = false;
    }
    if(age!=""){
        bool ok;
        int tage=age.toInt(&ok,10);
        if(!ok||tage<0||tage>300){
            tips("错误","请填入合法的年龄，谢谢!!!");
            return;
        }
        if(!first){
            sql = sql + ",";
        }
        sql = sql + " age = '" + age + "'";
        first = false;
    }
    if(personIntroduce!=""){
        if(!first){
            sql = sql + ",";
        }
        sql = sql + " personIntroduce = '" + personIntroduce +"'";
        first = false;
    }
    if(mail!=""){
        if(!isLegalMail(mail)){
            tips("错误","请输入合法的邮件地址!!!!");
            return;
        }
        if(!first){
            sql = sql + ",";
        }
        sql = sql + " mail = '" + mail +"'";
        first = false;
    }
    updateSQL(sql,first,"address",address);
    updateSQL(sql,first,"postCode",postCode);
    updateSQL(sql,first,"telephone",telePhone);
    updateSQL(sql,first,"fixPhone",officePhone);
    updateSQL(sql,first,"isReceiveMsg",isReceiveMsg);
    updateSQL(sql,first,"country",country);
    updateSQL(sql,first,"province",province);
    updateSQL(sql,first,"city",city);
    updateSQL(sql,first,"detailAddress",detailAddress);
    updateSQL(sql,first,"detailPersonInfo",detailPersonInfo);
    updateSQL(sql,first,"sex",sex);
    sql = sql + " where accountName = '" + userName +"'";
    QSqlQuery query(sql);
    if(query.exec()){
        tips("成功","信息更新成功！！！");
        changeInfoShowState();
    }else{
        tips("错误","数据库错误，"+query.lastError().text()+"!!!请稍后再试");
        changeInfoShowState();
    }
}

void MainWindow_sale::updatePhoto()
{
    QString imagePath = QFileDialog::getOpenFileName(this,"请选择图像","../../","图像文件(*.png *.jpg *.bmp *.gif)");
    if(imagePath.isNull()){
        return;
    }
    QString currentDir = QDir(imagePath).absolutePath();
    ui->labelPic->setPixmap(QPixmap(imagePath).scaled(ui->labelPic->size()));
    //保存到数据库
    if(!ui->labelPic->pixmap()->isNull()){
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        ui->labelPic->pixmap()->save(&buffer,"PNG");
        QString out;
        for(int k = 0; k < (int)bytes.size(); ++k){
            QString str;
            str.sprintf("%02x",(uchar)bytes[k]);
            out.append(str);
        }
        QSqlQuery query(QString("update usrinfo set photo = 0x%1 where accountName = '%2' and workNumber = '%3'")\
                        .arg(out).arg(userName).arg(wkNumber));
        if(!query.exec()){
            QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
            return ;
        }
        QMessageBox::information(0,"成功","头像更新成功",QMessageBox::Ok);
    }
}

void MainWindow_sale::clearPwd()
{
    ui->lineEditPwd->setText("");
    ui->lineEditNewPwd1->setText("");
    ui->lineEditNewPwd2->setText("");
}

void MainWindow_sale::updatePwd()
{
    QSqlQuery query;
    if(!checkIsSame(ui->lineEditNewPwd1->text(),ui->lineEditNewPwd2->text())){
        QMessageBox::information(this,"错误","两次输入密码不一致，请重新输入!",QMessageBox::Ok);
        clearPwd();
        return;
    }
    QString sql = "select * from usrsaler where accountName = '" + userName + "'";
    query.exec(sql);
    //qDebug() << userName << " " << query.size();
    QString oldPwd;
    if(query.next())oldPwd = query.value("passWord").toString();
    if(oldPwd != getMd5Pwd(ui->lineEditPwd->text())){
        QMessageBox::information(this,"错误","密码错误，请重新输入!",QMessageBox::Ok);
        clearPwd();
        return;
    }
    QString md5Pwd = getMd5Pwd(ui->lineEditNewPwd1->text());

    if(!query.exec("update usradmin set passWord = '"+md5Pwd+"';")){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+"!请稍后再试",QMessageBox::Ok);
        return;
    }
    else{
        QMessageBox::information(0,"成功","密码修改成功!!!",QMessageBox::Ok);
        return;
    }
}

void MainWindow_sale::returnToLogin()
{
    if(QMessageBox::question(this,"提示","是否注销用户?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
        emit toLoginWindow();
        this->close();
        //delete this;
    }
}

void MainWindow_sale::exitSys()
{
    if(QMessageBox::question(this,"提示","是否退出系统?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
        this->close();
        //delete this;
    }
}

void MainWindow_sale::setInfo(QString *acName)
{
    userName = *acName;
}

bool MainWindow_sale::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->menuShangPinXiaoShou){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidgetSale->setCurrentIndex(0);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else if(obj == ui->menuZhangHuGuanLi){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidgetSale->setCurrentIndex(1);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }
    else if(obj == ui->menuHelp){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidgetSale->setCurrentIndex(2);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else{
        return QMainWindow::eventFilter(obj,e);
    }
}

void MainWindow_sale::keyPressEvent(QKeyEvent *e)
{
    QSqlQuery query;
    QString str,sMerchantNumber,sMerchantCode,sql,sName,tStr,filePath;
    int nCurrentRow,nNum,nTmp;
    double dPrice,dRes;
    memberDialog *reg = new memberDialog(this);
    QueryMerchantDialog *search = new QueryMerchantDialog(this);
    QMap<QString,int> map;
    QStringList merchant;
    QVariantList orderNumber,merchantNumber,number,discount;
    bool isVIP = false;


    switch(e->key()){
    case Qt::Key_F1://选择该商品
        sMerchantNumber = ui->lineEditBianHao->text();
        sMerchantCode = ui->lineEditTiaoMa->text();
        if(sMerchantNumber==""&&sMerchantCode==""){
            tips("错误1","商品编号或条码应至少填一个");
            return ;
        }

        nNum = ui->spinBoxNum->text().toInt();
        if(nNum<=0){
            tips("错误2","至少要购买一件商品");
            return ;
        }
        if(sMerchantNumber!=""&&sMerchantCode!=""){
            sql = "select * from merchant where merchantNumber='"+sMerchantNumber+\
                    "' and merchantcode = '"+sMerchantCode+"'";
        }else if(sMerchantNumber==""&&sMerchantCode!=""){
            sql = "select * from merchant where merchantCode='"+sMerchantCode+"'";
        }
        else{
            sql = "select * from merchant where merchantNumber='"+sMerchantNumber+"'";
        }
        if(!query.exec(sql)){
            tips("错误3","数据库错误");
            return ;
        }
        if(query.size()==0){
            tips("错误4","输入有误，请重新输入！！");
            return;
        }
//        qDebug() << sql << " this is F1 " << price;
        if(query.next()){
            sName = query.value("merchantName").toString();
            sMerchantCode = query.value("merchantCode").toString();
        }
        nCnt += nNum;
        accMoney = accMoney + ui->labelMoneySum->text().toDouble();
        str.sprintf("%.2f",accMoney);
        ui->labelMoney_2->setText(str);
        ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
        ui->labelXiaoFeiXiaoFeiJinE_2->setText(str);
        nCurrentRow = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(nCurrentRow + 1);

        ui->tableWidget->setItem(nCurrentRow,0,new QTableWidgetItem(sMerchantNumber));
        ui->tableWidget->setItem(nCurrentRow,1,new QTableWidgetItem(sName));
        ui->tableWidget->setItem(nCurrentRow,2,new QTableWidgetItem(sMerchantCode));
        ui->tableWidget->setItem(nCurrentRow,3,new QTableWidgetItem(QString("%1").arg(price)));
        if(isVIP)ui->tableWidget->setItem(nCurrentRow,4,new QTableWidgetItem("0.2"));
        else ui->tableWidget->setItem(nCurrentRow,4,new QTableWidgetItem("0"));
        ui->tableWidget->setItem(nCurrentRow,5,new QTableWidgetItem(QString("%1").arg(nNum)));
        ui->tableWidget->setItem(nCurrentRow,7,new QTableWidgetItem(QDate::currentDate().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(nCurrentRow,6,new QTableWidgetItem(ui->labelMoneySum->text()));
        ui->tableWidget->setItem(nCurrentRow,8,new QTableWidgetItem(userName));
        nAction = 1;
        //有时间在写把重复的加一块
        break;

    case Qt::Key_F2://会员注册
//        if(reg->exec()==QDialog::Accepted){
//            tips("提示","注册成功v_v");
//        }else{
//            tips("提示","注册失败^_^");
//        }
        reg->exec();
        break;

    case Qt::Key_F3://赠品选择
        if(ui->lineEditHuiYuanNumber->text()==""){
            tips("错误","请输入会员编号");
            return;
        }
        if(ui->zengPingListWidget->count()>=1){
            ui->zengPingListWidget->setCurrentRow((ui->zengPingListWidget->currentRow()+1)\
                                                  %ui->zengPingListWidget->count());
        }
        break;

    case Qt::Key_F4://导入
        filePath = QFileDialog::getOpenFileName(this,"请选择文件","../../","表格文件(*.xlsx)");
        if(filePath.isNull()){
            return;
        }
        tips("提示","系统暂时不支持此项功能");
        //后面再处理此功能
        break;

    case Qt::Key_F5://结算
        if(ui->tableWidget->rowCount()==0){
            tips("提示","你还没有输入任何商品(⊙o⊙)?");
            return;
        }
        //写数据库,更改数据库信息
        if(ui->lineEditHuiYuanNumber->text()!=""){
            //需要输入密码
            ShowPassWord *password = new ShowPassWord(ui->lineEditHuiYuanNumber->text(),this);
//            QDialog *password = new QDialog(this);
//            QLabel *lTip = new QLabel("密码：");
//            QLineEdit *leInput = new QLineEdit();
//            QPushButton *ok = new QPushButton("确定");
//            QPushButton *cancel = new QPushButton("取消");
//            QHBoxLayout *h1 = new QHBoxLayout();
//            h1->addWidget(lTip);
//            h1->addSpacing(10);
//            h1->addWidget(leInput);
//            QHBoxLayout *h2 = new QHBoxLayout();
//            h2->addSpacing(10);
//            h2->addWidget(ok);
//            h2->addSpacing(20);
//            h2->addWidget(cancel);
//            h2->addSpacing(10);
//            QVBoxLayout *v1 = new QVBoxLayout();
//            v1->addLayout(h1);
//            v1->addLayout(h2);
//            password->setLayout(v1);
//            password->setMinimumSize(256,128);
//            password->setMaximumSize(256,128);
//            //点击确定
            isVIP = true;
            if(password->exec()!=QDialog::Accepted){
                tips("错误","密码错误");
                return;
            }
        }
        //没有密码，或密码正确,存库售货

        //先生成订单数据，插入数据库;
        sql = "insert into saleMerchantOrder values('";
        sql = sql + ui->labelLiuShuiNumber1->text()+"','";//订单编号
//        if(ui->lineEditHuiYuanNumber->text()!="")//会员编号
        sql = sql + ui->lineEditHuiYuanNumber->text() +"','";
        sql = sql + QDate::currentDate().toString("yyyy-MM-dd") + "','";
        sql = sql + userName + "');";
        if(!query.exec(sql)){
            tips("错误1","数据库出错");
            return;
        }
        nTmp = nCurrentRow = 0;
        map.clear();
        for(int i=0;i<ui->tableWidget->rowCount();++i){
            tStr = ui->tableWidget->item(i,0)->text();
            if(!map.contains(tStr)){
                map[tStr] = ui->tableWidget->item(i,5)->text().toInt();
                merchant.append(tStr);
            }else{
                map[tStr] = map[tStr] + ui->tableWidget->item(i,5)->text().toInt();
            }
        }
//        qDebug() << merchant ;
        query.prepare("insert into custommerchant values(?,?,?,?)");
        for(int i=0;i<merchant.size();++i){
            orderNumber << ui->labelLiuShuiNumber1->text();
            merchantNumber << merchant.at(i);
            number << map[merchant.at(i)];
            discount << (isVIP ? "0.2":"0");
        }
//        qDebug() << orderNumber << " " << merchantNumber << " " << number << " " << discount;
        query.addBindValue(orderNumber);
        query.addBindValue(merchantNumber);
        query.addBindValue(number);
        query.addBindValue(discount);
        if(!query.execBatch()){
            tips("错误2","数据库出错");
            //回滚
            query.exec("delete from saleMerchantOrder where orderNumber = '"+\
                       ui->labelLiuShuiNumber1->text()+"'");
            return;
        }

        //从货架上减去
        for(QMap<QString,int>::const_iterator i= map.constBegin();i!=map.constEnd();++i){
            query.exec(QString("update goodsShelve set number = number - %1 where merchantNumber = '%2'")\
                       .arg(i.key()).arg(i.value()));
        }
        //清空表格，置零
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        //ui->tableWidget->setColumnCount(8);
        accMoney = price = 0.00;
        nCnt = 0;
        isVIP = false;
        initShow();//需要刷新显示
        break;

    case Qt::Key_F6://换班
        returnToLogin();
        break;

    case Qt::Key_F7://撤销,删除最后添加的一行或增加开始删除的一行
        if(nAction==1){
            nCurrentRow = ui->tableWidget->rowCount();
            if(nCurrentRow==0){
                tips("提示","你还没输入任何商品呢:-D");
                return;
            }
            nCurrentRow -= 1;
            nTmp = ui->tableWidget->item(nCurrentRow,5)->text().toInt();
            nCnt -= nTmp;
            ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
            dPrice = ui->tableWidget->item(nCurrentRow,3)->text().toDouble();
            dRes = ui->labelMoney_2->text().toDouble();
            dRes -= dPrice * (double)nTmp;
            tStr.sprintf("%.2lf",dRes);
            ui->labelMoney_2->setText(tStr);
            ui->labelXiaoFeiXiaoFeiJinE_2->setText(tStr);
            ui->tableWidget->setRowCount(nCurrentRow);
            ui->tableWidget->removeRow(nCurrentRow);
        }
        else if(nAction == 2){
            int nCurrentRow = ui->tableWidget->rowCount();
//            qDebug() << nCurrentRow << " " << selectedRow;
            ui->tableWidget->setRowCount(nCurrentRow + 1);
            for(int i=0;i<ui->tableWidget->rowCount();++i){
//                qDebug() << slTmp.at(i);
                ui->tableWidget->setItem(nCurrentRow,i,new QTableWidgetItem(sRe[i]));
            }
            nTmp = ui->tableWidget->item(nCurrentRow,5)->text().toInt();
            nCnt += nTmp;
            ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
            dPrice = ui->tableWidget->item(nCurrentRow,3)->text().toDouble();
            dRes = ui->labelMoney_2->text().toDouble();
            dRes += dPrice * (double)nTmp;
            tStr.sprintf("%.2lf",dRes);
            ui->labelMoney_2->setText(tStr);
            ui->labelXiaoFeiXiaoFeiJinE_2->setText(tStr);
        }else{
            tips("提示","不能再撤销了(⊙o⊙)哦");
        }
        nAction = -1;
        break;

    case Qt::Key_F8://会员充值
        if(ui->lineEditHuiYuanNumber->text()==""){
            tips("提示","请输入会员号码");
            return;
        }
        if(!query.exec("select * from VIPTable where VIPNumber = '"+ui->lineEditHuiYuanNumber->text()+"'")){
            tips("错误","数据库出错");
            return;
        }
        if(query.size()==0){
            tips("错误","不存在此会员，请注册");
            return;
        }
        if(!query.exec("update VIPTable set money = money + "+ui->spinBoxHYCZ->text())){
            tips("错误","数据库出错");
            return;
        }else{
            query.exec("select money from VIPTable where VIPNumber = '"+ui->lineEditHuiYuanNumber->text()+"'");
            if(query.next())tStr = query.value("money").toString();
            tips("成功","充值成功,你的余额为"+tStr);
            ui->labelHuiYuanJinE_2->setText(tStr);
            return;
        }
        break;

    case Qt::Key_F9://增加或减少商品数量
        //判断是否选中一行
        if(selectedRow<0||selectedRow>=ui->tableWidget->rowCount()){
            tips("提示","你没选择要修改的商品");
            return;
        }
        nCnt += 1;
        ui->tableWidget->item(ui->tableWidget->currentRow(),5)->setText(\
                    tStr.sprintf("%d",ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text().toInt() + 1));
        ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
        dPrice = ui->tableWidget->item(selectedRow,3)->text().toDouble();
        dRes = ui->labelMoney_2->text().toDouble();
        dRes += dPrice;
        tStr.sprintf("%.2lf",dRes);
        ui->labelMoney_2->setText(tStr);
        ui->labelXiaoFeiXiaoFeiJinE_2->setText(tStr);
        break;

    case Qt::Key_F11:
        //判断是否选中一行
        if(selectedRow<0||selectedRow>=ui->tableWidget->rowCount()){
            tips("提示","你没选择要修改的商品");
            return;
        }
        if(ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text().toInt() ==1){
            tips("提示","数量不能在减少了(*@ο@*) 哇～");
            return;
        }
        nCnt -= 1;
        ui->tableWidget->item(ui->tableWidget->currentRow(),5)->setText(\
                    tStr.sprintf("%d",ui->tableWidget->item(ui->tableWidget->currentRow(),5)->text().toInt() - 1));
        ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
        dPrice = ui->tableWidget->item(selectedRow,3)->text().toDouble();
        dRes = ui->labelMoney_2->text().toDouble();
        dRes -= dPrice;
        tStr.sprintf("%.2lf",dRes);
        ui->labelMoney_2->setText(tStr);
        ui->labelXiaoFeiXiaoFeiJinE_2->setText(tStr);
        break;

    case Qt::Key_F10://查询商品
        search->exec();
        break;

    case Qt::Key_Delete://删除一行
        nCurrentRow = ui->tableWidget->currentRow();
        if(nCurrentRow<0||nCurrentRow>=ui->tableWidget->rowCount()){
            tips("提示","请选择你要删除的行");
            ui->tableWidget->selectRow(0);
            return;
        }
        nTmp = ui->tableWidget->item(nCurrentRow,5)->text().toInt();
        nCnt -= nTmp;
        ui->labelMerchantNumber_2->setText(QString("%1").arg(nCnt));
        dPrice = ui->tableWidget->item(nCurrentRow,3)->text().toDouble();
        dRes = ui->labelMoney_2->text().toDouble();
        dRes -= dPrice * (double)nTmp;
        tStr.sprintf("%.2lf",dRes);
        ui->labelMoney_2->setText(tStr);
        ui->labelXiaoFeiXiaoFeiJinE_2->setText(tStr);


        for(int i=0;i<ui->tableWidget->columnCount();++i){
            sRe[i] = ui->tableWidget->item(nCurrentRow,i)->text();
        }
        ui->tableWidget->removeRow(nCurrentRow);
        //ui->tableWidget->setRowCount(ui->tableWidget->rowCount() - 1);
        nAction = 2;
        break;

    case Qt::Key_Escape://撤销此次购买，但是不存数据库
        //清空表格，置零
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        //ui->tableWidget->setColumnCount(8);
        accMoney = price = 0.00;
        nCnt = 0;
        isVIP = false;
        initShow();
        break;
    case Qt::Key_Enter:
        this->focusNextPrevChild(true);
        break;
    }
    return ;
}
