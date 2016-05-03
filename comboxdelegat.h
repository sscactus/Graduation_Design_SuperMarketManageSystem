#ifndef COMBOXDELEGAT_H
#define COMBOXDELEGAT_H

#include <QItemDelegate>

class ComboxDelegat : public QItemDelegate
{
    Q_OBJECT
public:
    ComboxDelegat(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem \
                          &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor,const QModelIndex &index)const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index)const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,\
                              const QModelIndex &index)const;
};

#endif // COMBOXDELEGAT_H
