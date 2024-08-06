#include "kshape.h"
#include "kglobaldata.h"
KShape::KShape(QObject *parent)
	: QObject(parent)
{
}

KShape::KShape(const KShape& other)
{
	m_startPoint = other.m_startPoint;
	m_endPoint = other.m_endPoint;
	m_penStyle = other.m_penStyle;
	m_borderColor = other.m_borderColor;
	m_fillColor = other.m_fillColor;
	m_borderWidth = other.m_borderWidth;
}


KShape::~KShape()
{
}

void KShape::move(QPoint offset)
{
	m_startPoint += offset;
	m_endPoint += offset;
}

void KShape::moveTop(QPoint pos)
{
	m_startPoint.ry() = pos.y();
}

void KShape::moveBottom(QPoint pos)
{
	m_endPoint.ry() = pos.y() ;
}

void KShape::moveLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x() ;
}

void KShape::moveRight(QPoint pos)
{
	m_endPoint.rx() = pos.x() ;
}

void KShape::moveTopLeft(QPoint pos)
{
	m_startPoint = pos;
}

void KShape::moveTopRight(QPoint pos)
{
	m_startPoint.ry() = pos.y();
	m_endPoint.rx() = pos.x();
}

void KShape::moveBottomLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x() ;
	m_endPoint.ry() = pos.y() ;
}

void KShape::moveBottomRight(QPoint pos)
{
	m_endPoint = pos ;
}

// 绘制选中矩形框
void KShape::drawOutLine(QPaintDevice* parent)
{
	QPainter painter(parent);
	QPen pen;

	pen.setColor(QColor(79, 128, 255));
	pen.setWidth(3);
	painter.setPen(pen);
	painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
	QPoint start = getStartPoint();
	QPoint end = getEndPoint();

	painter.drawRect(QRect(getStartPoint(), getEndPoint()));
}

QPoint KShape::getStartPoint()
{
	return m_startPoint;
}

QPoint KShape::getEndPoint()
{
	return m_endPoint;
}


void KShape::setStartPoint(const QPoint & point)
{
	m_startPoint = point;
}

void KShape::setEndPoint(const QPoint& point)
{
	m_endPoint = point;
}

KShapeType KShape::getShapeType()
{
	return KShapeType();
}

QRect KShape::getShapeRect() const
{
	return QRect(m_startPoint ,m_endPoint );
}

void KShape::updateParams()
{
	m_penStyle = KGlobalData::getGlobalDataIntance()->getBorderStyle();
	m_borderColor = KGlobalData::getGlobalDataIntance()->getShapeBorderColor();
	m_fillColor = KGlobalData::getGlobalDataIntance()->getShapeFillColor();
	m_borderWidth = KGlobalData::getGlobalDataIntance()->getBorderWidth();
}




bool KShape::isValid()
{
	return (m_startPoint.x() != m_endPoint.x() || m_startPoint.y() != m_endPoint.y());
}

void KShape::resetStartPoint(QPoint point)
{
	QPoint offset = point - m_startPoint;
	move(offset);
}