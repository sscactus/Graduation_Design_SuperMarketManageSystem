#ifndef MEMBERDIALOG_H
#define MEMBERDIALOG_H

#include <QDialog>

namespace Ui {
class memberDialog;
}

class memberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit memberDialog(QWidget *parent = 0);
    ~memberDialog();
private:
    void setTips();
private slots:
    void clear();
    void registerAccount();
    void enableReg(QString);


private:
    Ui::memberDialog *ui;
};

#endif // MEMBERDIALOG_H
