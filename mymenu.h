#ifndef MYMENU_H
#define MYMENU_H
#include <QMenu>
#include <QMouseEvent>


class myMenu : public QMenu
{
    Q_OBJECT
public:
    myMenu(QString text);
    ~myMenu();
protected:
    void mouseReleaseEvent(QMouseEvent *e);
private :
signals:
    void clicked();

};

#endif // MYMENU_H
