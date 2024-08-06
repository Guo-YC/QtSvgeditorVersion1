#ifndef _K_ICONONLYDELEGATE_H_
#define _K_ICONONLYDELEGATE_H_

#include <QStyledItemDelegate>

class KIconOnlyDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;  // �̳й��캯��

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // !_K_ICONONLYDELEGATE_H_