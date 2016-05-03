#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QItemDelegate>


class cDateDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    cDateDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const \
                          QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor,const QModelIndex &index)const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index)const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,\
                              const QModelIndex &index)const;
};

#endif // DATEDELEGATE_H
