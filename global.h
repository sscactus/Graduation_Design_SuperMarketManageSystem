#ifndef GLOBAL_H
#define GLOBAL_H
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QDialog>
#include <QIcon>
#include <QPalette>
#include <QButtonGroup>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QPicture>
#include <QPixmap>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QByteArray>
#include <QCryptographicHash>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QLayout>
#include <QDate>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QRegExp>
#include <QBuffer>

bool checkIsSame(QString pwd1,QString pwd2);
bool checkPassWord(QSqlQuery &query,QString accountName,QString passWord,bool admin);

QString getMd5Pwd(QString passWord);

void tips(QString title,QString text);
bool isLegalMail(QString str);
void updateSQL(QString &sql,bool &first,QString s1,QString s2);


#endif // GLOBAL_H

