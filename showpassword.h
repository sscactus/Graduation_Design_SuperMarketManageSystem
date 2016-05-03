#ifndef SHOWPASSWORD_H
#define SHOWPASSWORD_H

#include <QDialog>

namespace Ui {
class ShowPassWord;
}

class ShowPassWord : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPassWord(QString ID,QWidget *parent = 0);
    ~ShowPassWord();

private:
    Ui::ShowPassWord *ui;
};

#endif // SHOWPASSWORD_H
