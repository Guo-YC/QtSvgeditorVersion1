#ifndef __K_HEXAGON_H__
#define __K_HEXAGON_H__

#define M_PI 3.14159265358979323846
#include "kshape.h"
#include <QPoint>
#include <QVector>

class KHexagon : public KShape
{
    Q_OBJECT

public:
    KHexagon(QObject* parent = nullptr);
    ~KHexagon();
    virtual void drawShape(QPaintDevice* parent) override;
    virtual KShape* clone() override;
    void setCenter(const QPoint& center);
    void setRadius(int radius);

    QVector<QPoint> calculateVerticesInRectangle(const QPoint& startPoint, const QPoint& endPoint);

protected:
    QPoint m_center;
    int m_radius;    

private:
    QVector<QPoint> m_vertices;
};

#endif // __K_HEXAGON_H__
