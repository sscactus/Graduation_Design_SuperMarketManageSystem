#ifndef OUTLIBDIALOG_H
#define OUTLIBDIALOG_H

#include <QDialog>

namespace Ui {
class outLibDialog;
}

class outLibDialog : public QDialog
{
    Q_OBJECT

public:
    explicit outLibDialog(QWidget *parent = 0);
    ~outLibDialog();

private:
    Ui::outLibDialog *ui;

private slots:
    void outLib();
};

#endif // OUTLIBDIALOG_H
