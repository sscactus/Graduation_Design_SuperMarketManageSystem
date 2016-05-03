#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "definition.h"
#include "outlibdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    infoState = true;
    changeInfoShowState();

    //设置窗口标题
    this->setWindowTitle("超市管理系统");
    setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
    //设置窗口背景
    this->setObjectName("mainWindow");
    this->setStyleSheet("#mainWindow{border-image:url(:/SuperMarketManageSystem/img/loginbk1.jpg);}");
//    QPalette palette;
//    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/SuperMarketManageSystem/img/bgp.jpg").scaled(this->size())));
//    ui->widgetChart->setPalette(palette);
    //qDebug() << this->size();
    //设置表格选中高亮
    ui->tableWidget->horizontalHeader()->setHighlightSections(true);
    ui->tableWidget->verticalHeader()->setHighlightSections(true);
    //
    ui->tableWidget->setStyleSheet("background-color:gray");
    ui->otherLabel_2->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    //ui->tableWidget->setStyle(QStyle::CE_ScrollBarSlider);


//    this->selectRow = -1;

//    //初始化图像显示
//    QSqlQuery query("select distinct(storenumber) from merchantstore");
//    if(!query.exec()){
//        tips("错误","数据库出错");
//        return;
//    }
//    ui->comboBoxStoreID->clear();
//    while(query.next()){
//        ui->comboBoxStoreID->addItem(query.value("storeNumber").toString());
//    }
//    ui->tableWidgetStoreInfo->setStyleSheet("background-color:gray");
//    ui->widgetChart->setShape(PieChartWidget::PieChart);
//    ui->widgetChart->setSelectedRow(selectRow);
//    ui->tableWidgetStoreInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
//    ui->tableWidgetStoreInfo->setColumnWidth(0,119);
//    ui->tableWidgetStoreInfo->setColumnWidth(1,119);
//    ui->tableWidgetStoreInfo->setEditTriggers(QTableWidget::NoEditTriggers);
//    ui->tableWidgetStoreInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidgetStoreInfo->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
//    ui->tableWidgetStoreInfo->horizontalHeader()->setHighlightSections(true);
//    ui->tableWidgetStoreInfo->verticalHeader()->setHighlightSections(true);
//    showImage(ui->comboBoxStoreID->currentText());
    connect(ui->comboBoxStoreID,static_cast<void(QComboBox::*)\
            (const QString &)>(&QComboBox::currentIndexChanged),this,showImage);
    connect(ui->pushButtonOutToShelve,ui->pushButtonOutToShelve->clicked,this,outLib);
    connect(ui->tableWidgetStoreInfo,ui->tableWidgetStoreInfo->itemSelectionChanged,[=](){
        if(ui->tableWidgetStoreInfo->currentRow()!=-1){
            selectRow = ui->tableWidgetStoreInfo->currentRow();
        }else{
            selectRow = -1;
        }
        showImage(ui->comboBoxStoreID->currentText());
//        qDebug() << selectRow;
    });
//    ui->menuBar->setMouseTracking(false);

    QStringList headerLabel;
    headerLabel << "商品编号" << "销售数量";
    ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
    ui->tableWidgetTongJiTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTongJiTable->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->labelUser->setText(userName);
    ui->labelTime->setText(QTime::currentTime().toString("yyyy-MM-dd hh:mm:ss ddd"));
    //设置定时器
    timelinetimer=new QTimer(this);
    timelinetimer->setInterval(100);
    timelinetimer->start();
    connect(timelinetimer,SIGNAL(timeout()),this,SLOT(settimeline()));

    //设置库存数量为只显示
    ui->lineEditMerchStore->setEnabled(false);

    showNumoOfStore();

    QStringList year;
    year<<""<<"2016"<<"2015"<<"2014"<<"2013"<<"2012";
    ui->comboBoxYear->addItems(year);
    QStringList month;
    month<<""<<"01"<<"02"<<"03"<<"04"<<"05"<<"06"<<"07"<<"08"<<"09"<<"10"<<"11"<<"12";
    ui->comboBoxMonth->addItems(month);

    //给menu安装时间监听器
    ui->menuTongjibaobiao->installEventFilter(this);
    ui->menuXitongshezhi->installEventFilter(this);
    ui->menuBangzhu->installEventFilter(this);
    ui->menuYewuguanli->installEventFilter(this);
    //ui->zhanghuguanli->installEventFilter(this);
    //设置初始显示界面
    initialShow();

    //绑定各个menu和其相应的显示界面
    connect(ui->zhuxiao,ui->zhuxiao->triggered,this,returnLogin);
    connect(ui->tuichu,ui->tuichu->triggered,this,exitSys);
    connect(ui->jinhuoguanli,ui->jinhuoguanli->triggered,this,this->showJinhuo);
    connect(ui->kucunguanli,ui->kucunguanli->triggered,this,this->showKucun);
    connect(ui->shangpinxinxiguanli,ui->shangpinxinxiguanli->triggered,this,this->showMerchaInfo);
    connect(ui->action_S,ui->action_S->triggered,this,showSaleTongji);
    connect(ui->action_A,ui->action_A->triggered,this,showSaleFenxi);
    connect(ui->zhigongxinxiguanli,ui->zhigongxinxiguanli->triggered,this,this->showStaffInfo);
    connect(ui->gonghuoshangxinxiguanli,ui->gonghuoshangxinxiguanli->triggered,this,this->showProviderInfo);
    //connect(menuXitongshezhi,menuXitongshezhi->clicked,this,this->showSystemSet);
    connect(ui->mimaxiugai,ui->mimaxiugai->triggered,this,this->showPasswordChange);
    connect(ui->gerenxinxi,ui->gerenxinxi->triggered,this,showPersonInfo);
    //connect(menuTongjibaobiao,menuTongjibaobiao->clicked,this,this->showStatisticRecord);
    //connect(menuYewuguanli,menuYewuguanli->clicked,this,this->showBusiness);
    //connect(menuBangzhu,menuBangzhu->clicked,this,this->showHelp);
    connect(ui->checkBoxGuarantee,ui->checkBoxGuarantee->stateChanged,
            [=](int s){
        if(s == 2)ui->lineEditDays->setEnabled(true);
        else ui->lineEditDays->setEnabled(false);
    });
    connect(ui->pushButtonUpdate,ui->pushButtonUpdate->clicked,this,this->updatePhoto);
    connect(ui->pushButtonPurchase,ui->pushButtonPurchase->clicked,this,this->addToLib);
    connect(ui->pushButtonClear,ui->pushButtonClear->clicked,this,this->clearAll);
    connect(ui->lineEditMerchNum,ui->lineEditMerchNum->textChanged,this,\
            this->showNumoOfStore);
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,this->changeInfoShowState);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,this->savePrsonInfo);
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,this->clearPwd);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,this->updatePwd);
    /*connect(ui->sexComboBox_2,ui->sexComboBox_2->currentIndexChanged,this,[=](int){
        QString sql = "update usrAdmin set sex = '" + ui->sexComboBox_2->currentText() + "';";
        QSqlQuery query(sql);
        if(!query.exec()){
            tips("错误","数据库出错，"+query.lastError().text()+"!!!请稍后再试");
        }
        else{
            tips("成功","更新信息成功!!!");
        }
    });*/
    connect(ui->radioButtonTongJiTime,ui->radioButtonTongJiTime->clicked,this,[=](){
        ui->groupBoxTongJitime->setEnabled(true);
        QStringList headerLabel;
        headerLabel << "商品编号" << "销售数量";
        ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
        ui->radioButtonPieChar->setChecked(true);
        ui->widgetTongJiChart->setShape(PieChartWidget::PieChart);
        ui->widgetTongJiChart->setIsPaint(false);
        ui->comboBoxTongJiInfo->setEnabled(false);
        setDateCom();
        showTimeChart();
    });
    connect(ui->radioButtonBarChart,ui->radioButtonBarChart->clicked,[=](){
        ui->widgetTongJiChart->setRefresh(PieChartWidget::BarChart);
    });
    connect(ui->radioButtonPieChar,ui->radioButtonPieChar->clicked,[=](){
        ui->widgetTongJiChart->setRefresh(PieChartWidget::PieChart);
    });
    connect(ui->comboBoxTongJiInfo,ui->comboBoxTongJiInfo->currentTextChanged,[=](QString s){
        if(ui->radioButtonTongJiMerchant->isChecked()){
            showMerchantChart(s);
        }
        else{
            showClerkChart(s);
        }
    });
    connect(ui->radioButtonTongJiClerk,ui->radioButtonTongJiClerk->clicked,[=](){
        ui->comboBoxTongJiInfo->setEnabled(true);
        ui->groupBoxTongJitime->setEnabled(false);
        QStringList headerLabel;
        headerLabel << "销售人员" << "销售数量";
        ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
        ui->radioButtonPieChar->setChecked(true);
        ui->widgetTongJiChart->setShape(PieChartWidget::PieChart);
        ui->widgetTongJiChart->setIsPaint(false);
        ui->comboBoxTongJiInfo->clear();
        ui->comboBoxTongJiInfo->addItem("所有");
        QSqlQuery query("select * from merchant");
        if(!query.exec()){
            tips("错误","数据库错误");
            return;
        }
        while(query.next()){
            ui->comboBoxTongJiInfo->addItem(query.value("merchantNumber").toString());
        }
        showClerkChart("所有");
    });
    connect(ui->radioButtonTongJiMerchant,ui->radioButtonTongJiMerchant->clicked,[=](){
        ui->groupBoxTongJitime->setEnabled(false);
        ui->comboBoxTongJiInfo->setEnabled(true);
        QStringList headerLabel;
        headerLabel << "商品编号" << "销售数量";
        ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
        ui->radioButtonPieChar->setChecked(true);
        ui->widgetTongJiChart->setShape(PieChartWidget::PieChart);
        ui->widgetTongJiChart->setIsPaint(false);
        ui->comboBoxTongJiInfo->clear();
        ui->comboBoxTongJiInfo->addItem("所有");
        QSqlQuery query("select * from usrSaler");
        if(!query.exec()){
            tips("错误","数据库错误");
            return;
        }
        while(query.next()){
            ui->comboBoxTongJiInfo->addItem(query.value("accountName").toString());
        }
        showMerchantChart("所有");
    });
    connect(ui->comboBoxYear,ui->comboBoxYear->currentTextChanged,[=](QString){
        setDateCom();
        showTimeChart();
    });
    connect(ui->comboBoxMonth,ui->comboBoxMonth->currentTextChanged,[=](QString){
        setDateCom();
        showTimeChart();
    });
    connect(ui->comboBoxDate,ui->comboBoxDate->currentTextChanged,[=](QString){showTimeChart();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage(QString storeID)
{
    QSqlQuery query("select merchantNumber,number from merchantStore where storeNumber = '"+storeID+"'");
    if(!query.exec()){
        tips("错误","数据库出错");
        return;
    }
    ui->widgetChart->setIsPaint(true);
    ui->widgetChart->setXY(ui->widgetChart->width()-160,ui->widgetChart->height()-120);
    int n = query.size();
    ui->tableWidgetStoreInfo->clearContents();
    ui->tableWidgetStoreInfo->setRowCount(n);
//    QStringList color = QColor::colorNames();
    QVector<double> values(n+1);
    QVector<QColor> colors(n+1);
    QVector<QString> labels(n+1);
    int k = 0;
    while(query.next()){
        values[k] = query.value("number").toString().toDouble();
        labels[k] = query.value("merchantNumber").toString();
        colors[k].setRgb(rgb[k][0],rgb[k][1],rgb[k][2]);
        ui->tableWidgetStoreInfo->setItem(k,0,new QTableWidgetItem(labels[k]));
        ui->tableWidgetStoreInfo->setItem(k,1,new QTableWidgetItem(QString("%1").arg(values[k])));
        ++k;
    }
    if(!query.exec("select leftCap from store where storeNumber = '"+storeID+"'")){
        tips("错误","数据库出错");
        return;
    }
    if(query.next()){
        values[k] = query.value(0).toString().toDouble();
        labels[k] = "剩余库存";
        colors[k].setRgb(rgb[39][0],rgb[39][1],rgb[39][2]);
    }
    if(selectRow>=0&&selectRow<n){
        ui->widgetChart->setSelectedRow(selectRow);
    }
    if(!query.exec("select * from store where storeNumber = '"+storeID+"'")){
        tips("错误","数据库出错");
        return;
    }
    if(query.next()){
        ui->textBrowserStoreInfo->setTextColor(Qt::red);
        ui->textBrowserStoreInfo->setText("************仓库信息************\n\n\n*********仓库编号\n\n     "+\
                                          query.value(0).toString()+"\n\n\n*********仓库负责人\n\n     "+\
                                          query.value(1).toString()+"\n\n\n**********仓库位置\n\n     "+\
                                          query.value(3).toString()+"\n\n\n*********仓库容量\n\n     "+\
                                          query.value(2).toString()+"\n\n\n*********仓库剩余容量\n\n    "+\
                                          query.value(4).toString()+"\n\n\n\n\n************仓库信息************");
    }
    ui->widgetChart->setData(values,colors,labels);
//    if(barImage!=NULL)delete barImage;
//    barImage = new ImagePanel(data,labels,ui->widget);

}

void MainWindow::outLib()
{
    outLibDialog *dialog=new outLibDialog(this);
    if(dialog->exec()==QDialog::Accepted)
    {
        refreshTable(1);
        showImage(ui->comboBoxStoreID->currentText());
    }
}

void MainWindow::showMerchantChart(QString s)
{
    QSqlQuery query;
    QString sql;
    if(s == "所有"){
        sql = "select merchantNumber,sum(number) as num from custommerchant group by merchantNumber";
    }else{
        sql = "SELECT merchantNumber,SUM(number) AS num FROM custommerchant WHERE orderNumber IN "
              "(SELECT orderNumber FROM salemerchantorder WHERE saler = '"+s+\
                "') GROUP BY merchantNumber";
    }
    if(!query.exec(sql)){
        tips("错误","数据库出错");
        return;
    }
    int n = query.size();
    QVector<double> values(n);
    QVector<QColor> color(n);
    QVector<QString> labels(n);
    ui->tableWidgetTongJiTable->clear();
    QStringList headerLabel;
    headerLabel << "商品编号" << "销售数量";
    ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
    ui->tableWidgetTongJiTable->setRowCount(n);
    ui->tableWidgetTongJiTable->setColumnCount(2);
    int k = 0;
    while(query.next()){
        values[k] = query.value("num").toDouble();
        color[k].setRgb(rgb[k][0],rgb[k][1],rgb[k][2]);
        labels[k] = query.value("merchantNumber").toString();
        ui->tableWidgetTongJiTable->setItem(k,0,new QTableWidgetItem(labels[k]));
        ui->tableWidgetTongJiTable->setItem(k,1,new QTableWidgetItem(QString("%1").arg(values[k])));
        ++k;
    }
    ui->widgetTongJiChart->setData(values,color,labels);
}

void MainWindow::showClerkChart(QString s)
{
    QSqlQuery query;
    QString sql;
    if(s == "所有"){
        sql = "SELECT saler,SUM(custommerchant.number) AS num FROM custommerchant,salemerchantorder"
              " WHERE custommerchant.orderNumber = salemerchantorder.orderNumber GROUP BY saler;";
    }else{
        sql = "SELECT saler,SUM(custommerchant.number) AS num FROM custommerchant,salemerchantorder"
              " WHERE custommerchant.orderNumber = salemerchantorder.orderNumber and merchant"
              "Number = '"+s+"' GROUP BY saler;";
    }
    if(!query.exec(sql)){
        tips("错误","数据库出错");
        return;
    }
    int n = query.size();
    QVector<double> values(n);
    QVector<QColor> color(n);
    QVector<QString> labels(n);
    ui->tableWidgetTongJiTable->clear();
    QStringList headerLabel;
    headerLabel << "销售人员" << "销售数量";
    ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
    ui->tableWidgetTongJiTable->setRowCount(n);
    ui->tableWidgetTongJiTable->setColumnCount(2);
    int k = 0;
    while(query.next()){
        values[k] = query.value("num").toDouble();
        color[k].setRgb(rgb[k][0],rgb[k][1],rgb[k][2]);
        labels[k] = query.value("saler").toString();
        ui->tableWidgetTongJiTable->setItem(k,0,new QTableWidgetItem(labels[k]));
        ui->tableWidgetTongJiTable->setItem(k,1,new QTableWidgetItem(QString("%1").arg(values[k])));
        ++k;
    }
    ui->widgetTongJiChart->setData(values,color,labels);
}

void MainWindow::showTimeChart()
{
    QString sql = "SELECT merchantNumber,SUM(number) as num FROM custommerchant,"
                  "salemerchantorder WHERE custommerchant.orderNumber = "
                  "salemerchantorder.orderNumber";
    if(ui->comboBoxYear->currentText()!=""){
        sql=sql+" AND year(salemerchantorder.saleDate)='"+ui->comboBoxYear->currentText()+"'";
    }
    if(ui->comboBoxMonth->currentText()!=""){
        sql=sql+" AND month(salemerchantorder.saleDate) = '"+ui->comboBoxMonth->currentText()+"'";
    }
    if(ui->comboBoxDate->currentText()!=""){
        sql=sql+" AND day(salemerchantorder.saledate) = '"+ui->comboBoxDate->currentText()+"'";
    }
    sql=sql+" group by merchantNumber";
    QSqlQuery query(sql);
    if(!query.exec()){
        tips("错误","数据库出错");
        return;
    }
    int n = query.size();
    QVector<double> values(n);
    QVector<QColor> color(n);
    QVector<QString> labels(n);
    ui->tableWidgetTongJiTable->clear();
    ui->tableWidgetTongJiTable->setRowCount(n);
    ui->tableWidgetTongJiTable->setColumnCount(2);
    int k = 0;
    while(query.next()){
        values[k] = query.value("num").toDouble();
        color[k].setRgb(rgb[k][0],rgb[k][1],rgb[k][2]);
        labels[k] = query.value("merchantNumber").toString();
        ui->tableWidgetTongJiTable->setItem(k,0,new QTableWidgetItem(labels[k]));
        ui->tableWidgetTongJiTable->setItem(k,1,new QTableWidgetItem(QString("%1").arg(values[k])));
        ++k;
    }
    ui->widgetTongJiChart->setData(values,color,labels);
}

void MainWindow::setDateCom()
{
    ui->comboBoxDate->clear();
    QStringList days;
    days<<"";
    for(int i=1;i<=31;++i){
        if(QDate::isValid(ui->comboBoxYear->currentText().toInt(),
                 ui->comboBoxMonth->currentText().toInt(),
                 i)){
            days<<QString::number(i);
        }
    }
    ui->comboBoxDate->addItems(days);
}

void MainWindow::changeInfoShowState()
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
    ui->lineEditPost->setEnabled(infoState);
    ui->lineEditTelephone->setEnabled(infoState);
    ui->textEditPeronDetail->setEnabled(infoState);
    ui->checkBoxIsR->setEnabled(infoState);
}

void MainWindow::savePrsonInfo()
{
    QString name = ui->nameLineEdit_2->text();
    QString sex = ui->sexComboBox_2->currentText();
    QString depar = ui->deparTextEdit_2->toPlainText();
    QString age = ui->ageLineEdit_2->text();
    QString personIntroduce = ui->textEditPersonIntroduce->toPlainText();
    QString mail = ui->lineEditMail->text();
    QString address = ui->lineEditAddress->text();
    QString postCode = ui->lineEditPost->text();
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

void MainWindow::setUserName(QString usr, QString workNumber)
{
    userName = usr;
    wkNumber = workNumber;
    //statusBar()->showMessage("欢迎您,"+userName);
    setUpPersonInfo(userName,workNumber);
    ui->labelUser->setText(userName);
}
void MainWindow::showNumoOfStore()
{
    QString account = ui->lineEditMerchNum->text();
    QString sql = "select Number from merchantstore where merchantNumber = '" + account +"'";
    QSqlQuery query;
#ifdef DEBUG_LOG
    QMessageBox::information(this,"tips","in the showNumOfStore",QMessageBox::Ok);
#endif
    if(!query.exec(sql)){
        QMessageBox::information(this,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
        return;
    }
    int res = 0;
    while (query.next()) {
        res += query.value("Number").toInt();
    }
    QString t;t.sprintf("%d",res);
    ui->lineEditMerchStore->setText(t);
}

void MainWindow::setUpWidgetInMerch()
{
    //设置默认日期和保质期天数不可用
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setCalendarPopup(true);
    ui->checkBoxGuarantee->setChecked(false);
    ui->lineEditDays->setEnabled(false);
    ui->tableWidget->clearContents();
    ui->tableWidget->setAutoScroll(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //读取数据库显示已存商品
    //sql语句
    QString qString = "SELECT merchant.MerchantNumber,merchantclass.MerchantClassNumber,"
                      "merchantName,storeNumber,MerchantSpecification,merchantstore.number,"
                      "color,PriceUnit FROM merchant,merchant_class,merchantstore,"
                      "merchantclass where merchant_class.merchantNumber = merchant.merchantnumber"
                      " and merchantStore.merchantNumber = merchant.merchantnumber and "
                      "merchant_class.MerchantClassNumber = merchantclass.MerchantClassNumber";
    QSqlQuery query;
    if(!query.exec(qString)){
        QMessageBox::information(this,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
        return;
    }
    ui->tableWidget->setRowCount(query.size());
    ui->tableWidget->setColumnCount(8);
    int k = 0;
    while(query.next()){
        for(int i=0;i<8;++i){
            QTableWidgetItem *t = new QTableWidgetItem(query.value(i).toString());
            ui->tableWidget->setItem(k,i,t);
        }
        ++k;
    }
}

void MainWindow::setUpPersonInfo(QString Id,QString wkNum)
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
        ui->lineEditPost->setText(query.value("postcode").toString());
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

void MainWindow::updatePhoto()
{
    QString imagePath = QFileDialog::getOpenFileName(this,"请选择图像","../../","图像文件(*.png *.jpg *.bmp *.gif)");
    if(imagePath.isNull()){
        return;
    }
//    QString currentDir = QDir(imagePath).absolutePath();
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

void MainWindow::returnLogin()
{
    if(QMessageBox::question(this,"提示","是否注销用户?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
        emit toLoginWindow();
        this->close();
        //delete this;
    }
}

void MainWindow::exitSys()
{
    if(QMessageBox::question(this,"提示","是否退出系统?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
        this->close();
        //delete this;
    }
}

void MainWindow::showJinhuo()
{
    refreshTable(0);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showKucun()
{
    this->selectRow = -1;

    ui->widgetChart->setSelectedRow(-1);
    ui->widgetChart->setShape(PieChartWidget::PieChart);
    //初始化图像显示
    QSqlQuery query("select storenumber from store");
    if(!query.exec()){
        tips("错误","数据库出错");
        return;
    }
    ui->comboBoxStoreID->clear();
    while(query.next()){
        ui->comboBoxStoreID->addItem(query.value("storeNumber").toString());
    }
    showImage(ui->comboBoxStoreID->currentText());
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showMerchaInfo()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::showSaleTongji()
{
    ui->stackedWidget->setCurrentIndex(3);
    int w = ui->groupBoxTongJiChartType->width(),h = ui->groupBoxTongJiChartType->height();
    ui->groupBoxTongJiChartType->setGeometry(ui->widgetTongJiChart->width()-w,
                                             ui->widgetTongJiChart->height()-h,
                                             w,
                                             h);
    ui->radioButtonTongJiMerchant->setChecked(true);
    QStringList headerLabel;
    headerLabel << "商品编号" << "销售数量";
    ui->tableWidgetTongJiTable->setHorizontalHeaderLabels(headerLabel);
    ui->radioButtonPieChar->setChecked(true);
    ui->groupBoxTongJitime->setEnabled(false);
    ui->widgetTongJiChart->setShape(PieChartWidget::PieChart);
    ui->widgetTongJiChart->setIsPaint(false);
    ui->comboBoxTongJiInfo->clear();
    ui->comboBoxTongJiInfo->addItem("所有");
    QSqlQuery query("select * from usrSaler");
    if(!query.exec()){
        tips("错误","数据库错误");
        return;
    }
    while(query.next()){
        ui->comboBoxTongJiInfo->addItem(query.value("accountName").toString());
    }
    QString saler = ui->comboBoxTongJiInfo->currentText();
    QString sql;
    if(saler == "所有"){
        sql = "select merchantNumber,sum(number) as num from custommerchant group by merchantNumber";
    }else{
        sql = "SELECT merchantNumber,SUM(number) AS num FROM custommerchant WHERE orderNumber IN "
              "(SELECT orderNumber FROM salemerchantorder WHERE saler = '"+saler+\
                "') GROUP BY merchantNumber";
    }
    if(!query.exec(sql)){
        tips("错误","数据库出错");
        return;
    }
    int n = query.size();
    QVector<double> values(n);
    QVector<QColor> color(n);
    QVector<QString> labels(n);
    ui->tableWidgetTongJiTable->setRowCount(n);
    ui->tableWidgetTongJiTable->setColumnCount(2);
    int k = 0;
    while(query.next()){
        values[k] = query.value("num").toDouble();
        color[k].setRgb(rgb[k][0],rgb[k][1],rgb[k][2]);
        labels[k] = query.value("merchantNumber").toString();
        ui->tableWidgetTongJiTable->setItem(k,0,new QTableWidgetItem(labels[k]));
        ui->tableWidgetTongJiTable->setItem(k,1,new QTableWidgetItem(QString("%1").arg(values[k])));
        ++k;
    }
    ui->widgetTongJiChart->setData(values,color,labels);
}

void MainWindow::showSaleFenxi()
{
    ui->widgetQuShiChart->setIsPaint(false);
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::showStaffInfo()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::showProviderInfo()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::showSystemSet()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::showPasswordChange()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->stackedWidget->setCurrentIndex(8);
    //disconnect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,changeInfoShowState);
    //disconnect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,savePrsonInfo);
    ui->pushButtonChange->disconnect();
    ui->pushButtonSave->disconnect();
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,clearPwd);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,updatePwd);
    ui->pushButtonSave->setEnabled(true);
    ui->pushButtonChange->setText("重置");
    ui->pushButtonSave->setText("确定");
}

void MainWindow::showPersonInfo()
{
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(8);
    //disconnect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,clearPwd);
    //disconnect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,updatePwd);
    ui->pushButtonChange->disconnect();
    ui->pushButtonSave->disconnect();
    connect(ui->pushButtonChange,ui->pushButtonChange->clicked,this,changeInfoShowState);
    connect(ui->pushButtonSave,ui->pushButtonSave->clicked,this,savePrsonInfo);

    ui->pushButtonChange->setText("修改");
    ui->pushButtonSave->setText("保存");
    if(infoState)changeInfoShowState();
}

void MainWindow::showStatisticRecord()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::showBusiness()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::showHelp()
{
    ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow::initialShow()
{
    connect(ui->accountManageListWidget,ui->accountManageListWidget->currentRowChanged,[=](int row){
       if(row<=1){
           if(row==1)showPasswordChange();
           else showPersonInfo();
           //ui->stackedWidget_2->setCurrentIndex(row);
       }else{
           if(row==2){
               returnLogin();
           }
           else{
               exitSys();
           }
       }
    });
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(8);
    ui->otherLabel_2->setVisible(true);
    setUpWidgetInMerch();
}

void MainWindow::clearAll()
{
    ui->lineEditDays->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEditInStaff->clear();
    ui->lineEditManufature->clear();
    ui->lineEditMerchClass->clear();
    ui->lineEditMerchCode->clear();
    ui->lineEditMerchColor->clear();
    ui->lineEditMerchCount->clear();
    ui->lineEditMerchCount_2->clear();
    ui->lineEditMerchInPrice->clear();
    ui->lineEditMerchName->clear();
    ui->lineEditMerchNum->clear();
    ui->lineEditMerchPropert->clear();
    ui->lineEditMerchPurchaseNum->clear();
    ui->lineEditMerchSalePrice->clear();
    ui->lineEditMerchStore->clear();
    ui->lineEditOrderNumber->clear();
}

void MainWindow::clearPwd()
{
    ui->lineEditPwd->setText("");
    ui->lineEditNewPwd->setText("");
    ui->lineEditNewPwd1->setText("");
}

void MainWindow::updatePwd()
{
    QSqlQuery query;
    if(!checkIsSame(ui->lineEditNewPwd->text(),ui->lineEditNewPwd1->text())){
        QMessageBox::information(this,"错误","两次输入密码不一致，请重新输入!",QMessageBox::Ok);
        clearPwd();
        return;
    }
    QString sql = "select * from usradmin where accountName = '" + userName + "'";
    query.exec(sql);
    //qDebug() << userName << " " << query.size();
    QString oldPwd;
    if(query.next())oldPwd = query.value("passWord").toString();
    if(oldPwd != getMd5Pwd(ui->lineEditPwd->text())){
        QMessageBox::information(this,"错误","密码错误，请重新输入!",QMessageBox::Ok);
        clearPwd();
        return;
    }
    QString md5Pwd = getMd5Pwd(ui->lineEditNewPwd->text());

    if(!query.exec("update usradmin set passWord = '"+md5Pwd+"';")){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+"!请稍后再试",QMessageBox::Ok);
        return;
    }
    else{
        QMessageBox::information(0,"成功","密码修改成功!!!",QMessageBox::Ok);
        return;
    }
}

//进货入库
void MainWindow::addToLib()
{
    QString merchantClass = ui->lineEditMerchClass->text();//类别
    QString merchantNumber = ui->lineEditMerchNum->text();//编号
    QString merchantName = ui->lineEditMerchName->text();//名字
    QString merchantCode = ui->lineEditMerchCode->text();//条码
    QString merchantSpecif = ui->lineEditMerchPropert->text();//规格型号
    QString merchantCount = ui->lineEditMerchCount->text();//计价单位
    QString merchantLimit = ui->lineEditMerchCount_2->text();//库存下限
    QString merchantColor = ui->lineEditMerchColor->text();//商品颜色
    QString merchantInPrice = ui->lineEditMerchInPrice->text();//商品进价
    QString merchantSalePrice = ui->lineEditMerchSalePrice->text();//商品售价
    QString merchantInNum = ui->lineEditMerchPurchaseNum->text();//商品购买数量
    QString merchantGuarantee = ui->lineEditDays->text();//保质期
    QString isCount = (ui->checkBoxCount->isChecked()?"1":"0");//是否计价称重
    QString other = ui->plainTextEditExtraInfo->toPlainText()==""?"无备注":ui->plainTextEditExtraInfo->toPlainText();//备注
    QString manufacture = ui->lineEditManufature->text();//生产厂商
    QString purchaseClerk = ui->lineEditInStaff->text();//进货员
    QString orderID = ui->lineEditOrderNumber->text();//订单号
    QString storeID = ui->lineEditStoreNumber->text();//仓库号
    QString inDate = ui->dateEdit->text();//进货日期
    if(merchantClass==""||merchantCode==""||merchantColor==""\
            ||merchantCount==""||merchantInNum==""||merchantInPrice==""\
            ||merchantLimit==""||merchantName==""||merchantNumber==""\
            ||merchantSalePrice==""||merchantSpecif==""||manufacture==""\
            ||purchaseClerk==""||orderID==""||storeID==""){
        QMessageBox::information(0,"错误","请填入有效数据!!!",QMessageBox::Ok);
        return;
    }
    if(!ui->lineEditDays->isEnabled()){
        merchantGuarantee = "10000";
    }
    //开始向数据库中插入数据
    QString sql = "";
    QSqlQuery query;
    query.exec("select * from store where StoreNumber = '"+storeID+"'");
    if(!query.size()){
        tips("错误","不存在该仓库，请重新填写");
        return ;
    }
    //查询是否有该类商品
    sql = "";
    if(query.exec("select * from merchantclass where merchantclassnumber = '"\
                  + merchantClass +"'")&&query.size() == 0){
        //不存在该类别，插入类别
        sql = sql+"insert into merchantclass(MerchantClassNumber,MerchantSpecification,"
                  "PriceUnit,leftNumber,saled) values('"+merchantClass+"','"+merchantSpecif+"','"\
                +merchantCount+"','0','0');";
    }
    if(query.exec("select * from merchant_class where merchantclassnumber = '"\
                  + merchantClass +"' and merchantNumber = '"+merchantNumber+"'")\
            &&query.size() == 0){
        sql = sql+"insert into merchant_class(MerchantNumber,MerchantClassNumber,number"
                  ") values('"+merchantNumber+"','"+merchantClass+"','0');";
    }
    if(query.exec("select * from merchantstore where storeNumber = '"+storeID+"' and merchantNumber = '"+merchantNumber+"'"))
    {
        if(query.size()==0){
            sql = sql+"insert into merchantStore values('"+merchantNumber+"','"+storeID+"','0');";
        }
        else{
            sql = sql+"update merchantStore set number = number + "+merchantInNum+"' where storeNumber ='"+\
                    storeID+"' and merchantNumber = '"+merchantNumber+"';";
        }

    }else{
        tips("错误","数据库出错");
        return;
    }
    if(query.exec("select * from merchant where merchantNumber = '"+merchantNumber+"'")&&
            query.size() == 0){
        //不存在该类商品，插入商品
        sql = sql+"insert into merchant(MerchantNumber,MerchantName,MerchantCode,MerchantManufacture,"
                  "MerchantGuarantte,isCount,color) values('"+merchantNumber+"','"+merchantName+"','"\
                +merchantCode+"','"+manufacture+"','"+merchantGuarantee+"','"+isCount+\
                "','"+merchantColor+"');";
//        sql = sql+"insert into merchantStore(MerchantNumber,StoreNumber,Number) values('"+merchantNumber+"','"
//                +storeID+"','"+merchantInNum+"');";
    }
    qDebug() << sql;
    if(sql!=""&&!query.exec(sql)){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
        return ;
    }
    sql = "";
    //假设都已经存在了
    sql = sql+"insert into inmerchantorder(orderNumber,MerchantNumber,MerchantNum,accountNumber,"
              "inDate,remark,inPrice,salePrice,limitNum) values('"+orderID+"','"+merchantNumber+"','"\
            +merchantInNum+"','"+purchaseClerk+"','"+inDate+"','"+other+"','"\
            +merchantInPrice+"','"+merchantSalePrice+"','"+merchantLimit+"');";//插入订单数据
    sql = sql+"update merchantclass set leftNumber = leftNumber + "+merchantInNum+\
            " where MerchantClassNumber = '" + merchantClass + "';";//修改该类商品剩余量
    sql = sql+"update merchant_class set number = number + "+merchantInNum+" where MerchantNumber = '"+\
            merchantNumber+"' and MerchantClassNumber = '"+merchantClass+"';";
    sql = sql+"update merchantStore set Number = Number + "+merchantInNum+\
            " where MerchantNumber = '" + merchantNumber + "' and storeNumber = '"\
            + storeID + "';";//修改该商品存在该库中的的数量
    sql = sql+"update store set Leftcap = leftCap - " + merchantInNum +" where storeNumber = '"+storeID+"';";
    qDebug() << sql;
    if(!query.exec(sql)){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+",请稍后再试",QMessageBox::Ok);
        return ;
    }
    else{
        QMessageBox::information(0,"成功","入库成功！",QMessageBox::Ok);
        refreshTable(1);
        return ;
    }
}
void MainWindow::refreshTable(bool bAddLib)
{
    int nCurrentRow = ui->tableWidget->rowCount();
    if(bAddLib){
        ui->tableWidget->setRowCount(nCurrentRow + 1);
        QSqlQuery query;
        QString qString = "SELECT merchant.MerchantNumber,merchantclass.MerchantClassNumber,"
                          "merchantName,storeNumber,MerchantSpecification,merchantstore.number,"
                          "color,PriceUnit FROM merchant,merchant_class,merchantstore,"
                          "merchantclass where merchant_class.merchantNumber = merchant.merchantnumber"
                          " and merchantStore.merchantNumber = merchant.merchantnumber and "
                          "merchant_class.MerchantClassNumber = merchantclass.MerchantClassNumber";
        if(!query.exec(qString)){
            tips("错误","数据库出错");
            ui->tableWidget->setRowCount(nCurrentRow);
            return;
        }
        ui->tableWidget->clearContents();
        int k = 0;
        while(query.next()){
            for(int i=0;i<8;++i){
                QTableWidgetItem *t = new QTableWidgetItem(query.value(i).toString());
                ui->tableWidget->setItem(k,i,t);
            }
            ++k;
        }
    }
}

void MainWindow::settimeline()
{
    QDateTime time=QDateTime::currentDateTime();
    QString str=time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->labelTime->setText(str);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->menuBangzhu){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidget->setCurrentIndex(11);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else if(obj == ui->menuYewuguanli){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidget->setCurrentIndex(10);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else if(obj == ui->menuTongjibaobiao){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidget->setCurrentIndex(9);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }

    }else if(obj == ui->zhanghuguanli){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->stackedWidget->setCurrentIndex(8);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else if(obj == ui->menuXitongshezhi){
        if(e->type() == QEvent::MouseButtonPress){
            ui->stackedWidget->setCurrentIndex(7);
            return true;
        }
        else{
            return QMainWindow::eventFilter(obj,e);
        }
    }else{
        return QMainWindow::eventFilter(obj,e);
    }
}
