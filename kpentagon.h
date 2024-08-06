#ifndef __K_PENTAGON_H__
#define __K_PENTAGON_H__

#define M_PI 3.14159265358979323846
#include "kshape.h"
#include <QPoint>
#include <QVector>

class KPentagon : public KShape
{
    Q_OBJECT

public:
    KPentagon(QObject* parent = nullptr);
    ~KPentagon();
    virtual void drawShape(QPaintDevice* parent) override;
    virtual KShape* clone() override;
    QVector<QPoint> calculateVerticesInRectangle(const QPoint& topLeft, const QPoint& bottomRight);

    KShapeType getShapeType();        
    

protected:
    QPoint m_center;
    int m_radius;

private:
    QVector<QPoint> m_vertices;
};

#endif // __K_PENTAGON_H__
