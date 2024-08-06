#ifndef __K_PENTAGRAM_H__
#define __K_PENTAGRAM_H__

#define M_PI 3.14159265358979323846
#include "kshape.h"
#include <QPoint>
#include <QVector>

class KPentagram : public KShape
{
    Q_OBJECT

public:
    KPentagram(QObject* parent = nullptr);
    ~KPentagram();
    virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
    virtual KShape* clone() override;
    virtual KShapeType getShapeType();

    QVector<QPoint> calculateVerticesInRectangle(const QPoint& startPoint, const QPoint& endPoint);

    QPoint findIntersection(const QPoint& p1, const QPoint& p2, const QPoint& p3, const QPoint& p4);

};

#endif // __K_PENTAGRAM_H__
