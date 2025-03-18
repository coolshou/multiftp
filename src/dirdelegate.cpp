#include "dirdelegate.h"

DirDelegate::DirDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void DirDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.column() == 1) {
        QVariant data = index.data();
        if (data.isValid()) {
            QString imagePath;
            if (data == 0){
                //upload
                imagePath = ":/upload";
            }else{
                imagePath = ":/download";
            }
            QImage image(imagePath);
            if (!image.isNull()) {
                QPixmap pixmap = QPixmap::fromImage(image);
                QRect rect = option.rect;
                painter->drawPixmap(rect, pixmap);
            }
        }
    }
}
