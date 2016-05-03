#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include "global.h"

namespace Ui {
class registerWindow;
}

class registerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit registerWindow(QWidget *parent = 0);
    ~registerWindow();
private:
    void setTips();
private slots:
    void clear();
    void registerAccount();
    void backToLogin();
    void enableReg(QString);
    void setAdminNumber();
    void hideAdminNumber();
signals:
    void toLoginDialog();

private:
    Ui::registerWindow *ui;
    QSqlTableModel *model;
    QButtonGroup *type;
};

#endif // REGISTERWINDOW_H
