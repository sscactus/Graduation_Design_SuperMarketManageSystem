#include "mymenu.h"
#include <QDebug>

myMenu::myMenu(QString text)
{
    this->setTitle(text);
}

myMenu::~myMenu()
{

}

void myMenu::mouseReleaseEvent(QMouseEvent *e)
{
    //qDebug<<e->x()<<" "<<e->y();
    emit clicked();
}
