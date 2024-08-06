#include "kpentagram.h"
#include "kglobaldata.h"
#include <QPainter>
#include <cmath>
#include <QPainterPath>
KPentagram::KPentagram(QObject* parent)
    : KShape(parent)
{
}

KPentagram::~KPentagram()
{
}


QPoint KPentagram::findIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4)
{
    int x1 = p1.x(), y1 = p1.y();
    int x2 = p2.x(), y2 = p2.y();
    int x3 = p3.x(), y3 = p3.y();
    int x4 = p4.x(), y4 = p4.y();

    int d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0) return QPoint();  // Lines are parallel

    int pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
    int x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
    int y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

    return QPoint(x, y);
}

void KPentagram::drawShape(QPaintDevice* parent)
{
    QPainter painter(parent);
    // 设置边框风格、颜色和宽度
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // 确保颜色代码是有效的
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);

    // 设置填充颜色
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());
    QVector<QPoint> m_vertices = calculateVerticesInRectangle(m_startPoint, m_endPoint);
    QVector<QPoint> starPoints(10);
    QVector<QPoint> innerVertices(5);
    for (int i = 0; i < 5; ++i) 
    {
        innerVertices[i] = findIntersection(
            m_vertices[i],
            m_vertices[(i + 2) % 5],
            m_vertices[(i + 1) % 5],
            m_vertices[(i + 3) % 5]
        );
    }
    
    for (int i = 0; i < 5; ++i) 
    {
        starPoints[2 * i] = m_vertices[i];
        starPoints[2 * i + 1] = innerVertices[(i+4)%5];
    }

    QPainterPath path;
    path.moveTo(starPoints[0]);
    for (int i = 1; i < 10; i++) 
    {
        path.lineTo(starPoints[i]);
    }
    path.closeSubpath();
    painter.drawPath(path);

}



KShapeType KPentagram::getShapeType()
{
    return KShapeType::PentagramShapeType;
}


QVector<QPoint> KPentagram::calculateVerticesInRectangle(const QPoint& startPoint, const QPoint& endPoint)
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

KShape* KPentagram::clone()
{
    KPentagram* p = new KPentagram(*this);
    p->setStartPoint(m_startPoint);
    p->setEndPoint(m_endPoint);
    return p;
}
