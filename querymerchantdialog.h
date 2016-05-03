#ifndef QUERYMERCHANTDIALOG_H
#define QUERYMERCHANTDIALOG_H

#include <QDialog>

namespace Ui {
class QueryMerchantDialog;
}

class QueryMerchantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryMerchantDialog(QWidget *parent = 0);
    ~QueryMerchantDialog();

private slots:
    void lookUp();

private:
    Ui::QueryMerchantDialog *ui;
};

#endif // QUERYMERCHANTDIALOG_H
