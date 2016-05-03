#ifndef MAINWINDOW_SALE_H
#define MAINWINDOW_SALE_H

#include "global.h"
#include "mymenu.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QHostInfo>
#include <QTableView>
#include <QStandardItemModel>

namespace Ui {
class MainWindow_sale;
}

class MainWindow_sale : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_sale(QWidget *parent = 0);
    ~MainWindow_sale();
    void setUserName(QString userName,QString workNumber);
private:
    void returnToLogin();
    void exitSys();
    void setUpPersonInfo(QString Id, QString wkNum);
    void initShow();

private slots:
    void setInfo(QString *acName);
    void changeInfoShowState();
    void savePrsonInfo();
    void updatePhoto();
    void clearPwd();
    void updatePwd();
    void getPriceFromID(QString sID);
    void getPriceFromCode(QString sCode);
    void showVIPInfo(QString sID);
    void calcuMoney(int nNum);
signals:
    void toLoginWindow();

private:
    Ui::MainWindow_sale *ui;
    QString userName,wkNumber;
    bool infoState;
    bool isVIP;
    double accMoney;
    double price;
    int nCnt;
    int selectedRow;
    int nAction;
    QString sRe[10];
    QStandardItemModel *model;

protected:
    bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_SALE_H
