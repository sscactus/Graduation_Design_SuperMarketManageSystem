#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "global.h"
#include "mainwindow_sale.h"
#include "mainwindow.h"
#include "registerwindow.h"

namespace Ui {
class loginWindow;
}

class loginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = 0);
    ~loginWindow();
private:
    void clear();
private slots:
    void exitBtnSlot();
    void loginBtnSlot();
    void regBtnSlot();
    void loginBtnSetSlot(QString);
    void changePicToAdmin();
    void changePicToSaler();
signals:
    //void showInfo(CString *acName);
private:
    Ui::loginWindow *ui;
    QButtonGroup *type;//记录登录人员类型
    QSqlTableModel *model;
};

#endif // LOGINWINDOW_H
