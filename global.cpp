
#include "global.h"


bool checkIsSame(QString pwd1,QString pwd2)
{
    return pwd1 == pwd2;
}

bool checkPassWord(QSqlQuery &query,QString accountName,QString passWord,bool admin)
{
    QString sql,user = admin?"usrAdmin":"usrSaler";
    qDebug() << user;
    sql = QString("select * from %1 where accountName = '%2'").arg(user).arg(accountName);
    qDebug() << sql;
    if(!query.exec(sql)){
        QMessageBox::information(0,"错误","数据库错误，"+query.lastError().text()+"!请稍后再试",QMessageBox::Ok);
        return false;
    }
    QString oldPwd;
    if(query.next())oldPwd = query.value("passWord").toString();
    QByteArray pwd = QCryptographicHash::hash(passWord.toUtf8(),QCryptographicHash::Md5);
    QString encodePassWord(pwd.toHex());
    qDebug() << oldPwd << " " << encodePassWord;
    return oldPwd == encodePassWord;
}

QString getMd5Pwd(QString passWord)
{
    QByteArray pwd = QCryptographicHash::hash(passWord.toUtf8(),QCryptographicHash::Md5);
    QString encodePassWord(pwd.toHex());
    return encodePassWord;
}

void tips(QString title,QString text)
{
    QMessageBox::information(0,title,text,QMessageBox::Ok);
}

bool isLegalMail(QString str)
{
    QString pattern("^[a-z0-9]+([._\\-]*[a-z0-9])*@([a-z0-9]+[-a-z0-9]*[a-z0-9]+.){1,63}[a-z0-9]+$");
    QRegExp rx(pattern);
    return rx.exactMatch(str);
}
void updateSQL(QString &sql,bool &first,QString s1,QString s2)
{
    if(!first){
        sql = sql + ",";
    }
    sql = sql + " " + s1 + " = '" + s2 + "'";
    first = false;
}
