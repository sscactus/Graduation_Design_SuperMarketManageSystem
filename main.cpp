#include "mainwindow.h"
#include "loginwindow.h"
#include "global.h"
#include <QApplication>
#include <QFile>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置窗口图标
    a.setWindowIcon(QIcon("logo.ico"));

    QFile iniFile("ini.txt");
    bool ok = iniFile.open(QIODevice::ReadOnly);
    QString driver,hostName,dataBaseName,userName,passWord;
    if(ok){
        QTextStream ts(&iniFile);
        if(!ts.atEnd()){
            QString line = ts.readLine();
            if(!line.isEmpty()){
                QStringList sl = line.split(",",QString::SkipEmptyParts);
                driver = sl.value(0);
                hostName = sl.value(1);
                dataBaseName = sl.value(2);
                userName = sl.value(3);
                passWord = sl.value(4);
            }
        }
    }
    else{
        driver = "QMYSQL";
        hostName = "127.0.0.1";
        dataBaseName = "SuperMarketManageSystem";
        userName = "root";
        passWord = "123456";
    }
    iniFile.close();
    //设置数据库
    QSqlDatabase user = QSqlDatabase::addDatabase(driver);
    user.setHostName(hostName);
    user.setDatabaseName(dataBaseName);
    user.setUserName(userName);
    user.setPassword(passWord);
    //连接数据库
    retry:
    if(user.open()){
        loginWindow w;
        w.setWindowFlags(Qt::WindowMinimizeButtonHint);
        w.show();

        return a.exec();
    }else{
        QMessageBox msgBox;
        msgBox.setText("系统连接数据库失败，"+user.lastError().text());
        msgBox.setInformativeText("你要再次连接么？");
        QPushButton *retryBtn = msgBox.addButton("retry",QMessageBox::ActionRole);
        QPushButton *cancelBtn = msgBox.addButton(QMessageBox::Cancel);
        msgBox.exec();
        if(msgBox.clickedButton() == retryBtn){
            goto retry;
        }
        else if(msgBox.clickedButton() == cancelBtn){
            return 0;
        }
    }
    return 0;
}
