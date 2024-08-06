#include "kcircle.h"
#include <QPainter>
#include "kglobaldata.h"
KCircle::KCircle(QObject* parent)
    : KShape(parent)
{
}

KCircle::~KCircle()
{
}

void KCircle::drawShape(QPaintDevice* parent)
{
    QPainter painter(parent);
    // 设置边框风格、颜色和宽度
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // 确保颜色代码是有效的
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
    // 设置填充颜色
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
    QPoint center = (getStartPoint() + getEndPoint()) / 2;
    int radius_x = ((getEndPoint().x() - getStartPoint().x()) / 2);
    int radius_y = ((getEndPoint().y() - getStartPoint().y()) / 2);
    painter.drawEllipse(center, radius_x, radius_y);
}

KShapeType KCircle::getShapeType()
{
    return KShapeType::CircleShapeType;
}

KShape* KCircle::clone()
{
    KCircle* p = new KCircle(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}