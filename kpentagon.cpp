#include "kpentagon.h"
#include "kglobaldata.h"
#include <QPainter>
#include <cmath>
#include <QTransform>

KPentagon::KPentagon(QObject* parent)
    : KShape(parent), m_center(100, 100), m_radius(50) // 默认中心和半径
{
}

KPentagon::~KPentagon()
{
}

void KPentagon::drawShape(QPaintDevice* parent)
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

    // 计算矩形的中心
    QPoint center((m_startPoint.x() + m_endPoint.x()) / 2, (m_startPoint.y() + m_endPoint.y()) / 2);
    // 计算矩形的宽度和高度
    int width = abs(m_endPoint.x() - m_startPoint.x());
    int height = abs(m_endPoint.y() - m_startPoint.y());

    // 使用宽度和高度计算五边形顶点
    QVector<QPoint> vertices = calculateVerticesInRectangle(m_startPoint, m_endPoint);
    //updateBounds(vertices);
    // 绘制五边形
    painter.drawPolygon(vertices.constData(), vertices.size());
}

QVector<QPoint> KPentagon::calculateVerticesInRectangle(const QPoint& startPoint, const QPoint& endPoint) 
{    
    QVector<QPoint> vertices;

    // Calculate the side length of the square
    int sideLength_x = std::abs(endPoint.x() - startPoint.x());
    int sideLength_y = std::abs(endPoint.y() - startPoint.y());

    // Calculate the coordinates of the square's corners
    QPoint topLeft = startPoint;
    QPoint topRight = QPoint(endPoint.x(), startPoint.y());
    QPoint bottomLeft = QPoint(startPoint.x(), endPoint.y());
    QPoint bottomRight = endPoint;

    // Calculate the coordinates of the five pentagon vertices
    QPoint topCenter((topLeft.x() + topRight.x()) / 2, topLeft.y());    
    QPoint leftVertex(topLeft.x(), topLeft.y() + sideLength_y * 0.412);
    QPoint rightVertex(topRight.x(), topRight.y() + sideLength_y * 0.412);
    QPoint bottomLeftVertex(bottomLeft.x() + sideLength_x * 0.191, bottomLeft.y());
    QPoint bottomRightVertex(bottomRight.x() - sideLength_x * 0.191, bottomRight.y());

    vertices.append(topCenter);
    vertices.append(leftVertex);
    vertices.append(bottomLeftVertex);
    vertices.append(bottomRightVertex);
    vertices.append(rightVertex);

    return vertices;
}

KShapeType KPentagon::getShapeType()
{
    return KShapeType::PentgaonShapeType;
}

KShape* KPentagon::clone()
{
    KPentagon* p = new KPentagon(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}