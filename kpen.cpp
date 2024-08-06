#include "kpen.h"
#include "kglobaldata.h"
#include <QPainter>
#include <QPoint>
#include <QDebug>
KPen::KPen(QObject* parent)
    : KShape(parent)
    
{

}

KPen::~KPen()
{
    
}

void KPen::drawShape(QPaintDevice* parent)
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
    if (!m_points.isEmpty()) 
    {
        for (int i = 1; i < m_points.size(); ++i) 
        {
            painter.drawLine(m_points[i - 1], m_points[i]);  // 绘制每一段线条
        }
    }
}

void KPen::addPoint(const QPoint& point)
{
    m_points.append(point);
    updateBounds(point);
}

void KPen::updateBounds(const QPoint& newPoint)
{
    if (m_points.size() == 1) 
    { // 如果是第一个点
        m_startPoint = newPoint;
        m_endPoint = newPoint;
    }
    else 
    {
        m_startPoint.setX(std::min(m_startPoint.x(), newPoint.x()));
        m_startPoint.setY(std::min(m_startPoint.y(), newPoint.y()));
        m_endPoint.setX(std::max(m_endPoint.x(), newPoint.x()));
        m_endPoint.setY(std::max(m_endPoint.y(), newPoint.y()));
    }
}



void KPen::setStartPoint(const QPoint& point)
{
    addPoint(point);    
}

void KPen::setEndPoint(const QPoint& point)
{
    addPoint(point);    
}

QRect KPen::getShapeRect() const
{    
    return QRect(m_startPoint, m_endPoint);
}

KShapeType KPen::getShapeType()
{
    return KShapeType::PenShapeType;
}

void KPen::move(QPoint offset)
{
    for (QPoint& point : m_points) 
    {
        point += offset;
    }
    m_startPoint += offset;
    m_endPoint += offset;
}

void KPen::moveTop(QPoint pos)
{
}

void KPen::moveBottom(QPoint pos)
{
}

void KPen::moveLeft(QPoint pos)
{
}

void KPen::moveRight(QPoint pos)
{
}

void KPen::moveTopLeft(QPoint pos)
{
}

void KPen::moveTopRight(QPoint pos)
{
}

void KPen::moveBottomLeft(QPoint pos)
{
}

void KPen::moveBottomRight(QPoint pos)
{
}


void KPen::updateBounds()
{
    if (m_points.isEmpty()) return;
    QPoint minPoint = m_points[0];
    QPoint maxPoint = m_points[0];
    for (const QPoint& point : m_points) 
    {
        if (point.x() < minPoint.x()) minPoint.setX(point.x());
        if (point.y() < minPoint.y()) minPoint.setY(point.y());
        if (point.x() > maxPoint.x()) maxPoint.setX(point.x());
        if (point.y() > maxPoint.y()) maxPoint.setY(point.y());
    }
    m_startPoint = minPoint;
    m_endPoint = maxPoint;
}

KShape* KPen::clone()
{
    KPen* p = new KPen(*this);
    for (int i = 0; i < m_points.size(); ++i)
    {
        p->addPoint(m_points[i]);
    }
    return p;
}

void KPen::resetStartPoint(QPoint point)
{
    QPoint offset = point - m_startPoint;
    KPen::move(offset);
}