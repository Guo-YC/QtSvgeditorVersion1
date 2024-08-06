#include "kline.h"
#include "kglobaldata.h"
#include <QPainter>
#include <QLine>
KLine::KLine(QObject *parent)
	: KShape(parent)
{
}

KLine::~KLine()
{
}

void KLine::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent); 
    // 设置边框风格、颜色和宽度
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // 确保颜色代码是有效的
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);

    // 设置填充颜色
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
	painter.drawLine(QLine(getStartPoint(),getEndPoint())); 
}

KShapeType KLine::getShapeType()
{
	return KShapeType::RectShapeType;
}


KShape* KLine::clone()
{
    KLine* p = new KLine(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}