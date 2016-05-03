#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"
#include "mymenu.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTableWidget>
//#include "imagepanel.h"

#define MAX_MERCHANT_TYPE 20


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUserName(QString usr,QString workNumber);
private:
    void initialShow();
    void setUpWidgetInMerch();
    void setUpPersonInfo(QString Id, QString wkNum);
    void showMerchantChart(QString s);
    void showClerkChart(QString s);
    void showTimeChart();
    void setDateCom();
    
private slots:
    void updatePhoto();
    void showJinhuo();
    void showKucun();
    void returnLogin();
    void exitSys();
    void showMerchaInfo();
    void showSaleTongji();
    void showSaleFenxi();
    void showStaffInfo();
    void showProviderInfo();
    void showSystemSet();
    void showPasswordChange();
    void showPersonInfo();
    void showStatisticRecord();
    void showBusiness();
    void showHelp();
    void clearAll();//重置入库所填的所有东西
    void addToLib();//将商品入库
    void refreshTable(bool bAddLib);//刷新表格显示库存
    void settimeline();
    void showNumoOfStore();//显示merchantNumber的库存数量
    void changeInfoShowState();//更改信息显示状态
    void savePrsonInfo();//保存信息
    void clearPwd();
    void updatePwd();
    void showImage(QString storeID);
    void outLib();

signals:
    void toLoginWindow();


private:
    Ui::MainWindow *ui;
    bool infoState;
    int selectRow;
    QString userName,wkNumber;
    QTimer *timelinetimer;
    myMenu *menuXitongshezhi;
    myMenu *menuBangzhu;
    myMenu *menuTongjibaobiao;
    myMenu *menuYewuguanli;
//    ImagePanel *barImage;

protected:
    bool eventFilter(QObject *obj, QEvent *e);
};

#endif // MAINWINDOW_H
