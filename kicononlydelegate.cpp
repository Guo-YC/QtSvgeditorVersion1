#include "kicononlydelegate.h"
#include <QPainter>
#include <QIcon>

void KIconOnlyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const 
{
    QRect rect = option.rect;
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QSize iconSize = icon.actualSize(rect.size());
    QPixmap pixmap = icon.pixmap(iconSize);
    QPoint point((rect.width() - iconSize.width()) / 2 + rect.x(),
        (rect.height() - iconSize.height()) / 2 + rect.y());
    painter->drawPixmap(point, pixmap);
}

QSize KIconOnlyDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const 
{
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    return icon.actualSize(option.rect.size());
}
