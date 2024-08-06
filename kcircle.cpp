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
    // ���ñ߿�����ɫ�Ϳ��
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // ȷ����ɫ��������Ч��
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
    // ���������ɫ
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