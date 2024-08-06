#include "krect.h"
#include "kglobaldata.h"
#include <QPainter>

KRect::KRect(QObject *parent)
	: KShape(parent)
{
}

KRect::~KRect()
{
}

void KRect::drawShape(QPaintDevice* parent)
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
	painter.drawRect(QRect(getStartPoint(),getEndPoint())); 
}

KShapeType KRect::getShapeType()
{
	return KShapeType::RectShapeType;
}


KShape* KRect::clone()
{
    KRect* p = new KRect(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}