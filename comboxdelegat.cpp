#include "comboxdelegat.h"
#include <QComboBox>

ComboxDelegat::ComboxDelegat(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *ComboxDelegat::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,\
                                     const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    for(int i=1;i<=20;++i){
        editor->addItem(QString("%1").arg(i));
    }
    editor->installEventFilter(const_cast<ComboxDelegat*>(this));
    return editor;
}

void ComboxDelegat::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();
    QComboBox *box = static_cast<QComboBox*>(editor);
    int i = box->findText(str);
    box->setCurrentIndex(i);
}

void ComboxDelegat::setModelData(QWidget *editor, QAbstractItemModel *model, const\
                                 QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();
    model->setData(index,str);
}

void ComboxDelegat::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem \
                                         &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
