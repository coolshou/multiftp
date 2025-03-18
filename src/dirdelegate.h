#ifndef DIRDELEGATE_H
#define DIRDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class DirDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DirDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DIRDELEGATE_H
