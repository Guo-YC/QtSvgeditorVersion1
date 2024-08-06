#include "khexagon.h"
#include <QPainter>
#include <cmath>
#include "kglobaldata.h"
KHexagon::KHexagon(QObject* parent)
    : KShape(parent), m_center(100, 100), m_radius(50)
{
}

KHexagon::~KHexagon()
{
}

void KHexagon::drawShape(QPaintDevice* parent)
{
    QPainter painter(parent);
    // 设置边框风格、颜色和宽度
    QPen pen;
    pen.setStyle(m_penStyle);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
    pen.setColor(QColor("#" + m_borderColor)); // 确保颜色代码是有效的
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);

    // 设置填充颜色
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
    m_vertices = calculateVerticesInRectangle(m_startPoint, m_endPoint);
    painter.drawPolygon(m_vertices.constData(), m_vertices.size());
}

void KHexagon::setCenter(const QPoint& center)
{
    m_center = center;
}

void KHexagon::setRadius(int radius)
{
    m_radius = radius;
}
QVector<QPoint> KHexagon::calculateVerticesInRectangle(const QPoint& startPoint, const QPoint& endPoint)
{
    QVector<QPoint> vertices;

    // Calculate the width and height of the rectangle
    int width = std::abs(endPoint.x() - startPoint.x());
    int height = std::abs(endPoint.y() - startPoint.y());

    // Calculate the margins if the hexagon is centered
    int marginX = width * 0.25;  // 10% of the width from each side
    int marginY = height * 0.5; // 20% of the height from top and bottom

    // Calculate the coordinates of the hexagon's vertices
    QPoint topCenterLeft(startPoint.x() + marginX, startPoint.y());
    QPoint topCenterRight(endPoint.x() - marginX, startPoint.y());
    QPoint bottomCenterLeft(startPoint.x() + marginX, endPoint.y());
    QPoint bottomCenterRight(endPoint.x() - marginX, endPoint.y());

    // Calculate the middle vertices on the left and right
    QPoint middleLeft(startPoint.x(), startPoint.y() + height / 2);
    QPoint middleRight(endPoint.x(), startPoint.y() + height / 2);

    // Append vertices to the vector in clockwise order
    vertices.append(topCenterLeft);
    vertices.append(topCenterRight);
    vertices.append(middleRight);
    vertices.append(bottomCenterRight);
    vertices.append(bottomCenterLeft);
    vertices.append(middleLeft);

    return vertices;
}


KShape* KHexagon::clone()
{
    KHexagon* p = new KHexagon(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}