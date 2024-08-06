#ifndef __K_PEN_H_
#define __K_PEN_H_

#include "kshape.h"

#include <QVector>
#include <QPoint>

class KPen : public KShape
{
    Q_OBJECT

public:
    KPen(QObject* parent = Q_NULLPTR);
    ~KPen();
    virtual void drawShape(QPaintDevice* parent) override;
    virtual KShape* clone() override;
    virtual void resetStartPoint(QPoint point) override;
    virtual void setStartPoint(const QPoint& point) override;
    virtual void setEndPoint(const QPoint& point) override;
    virtual QRect getShapeRect() const override;
    virtual KShapeType getShapeType() override;
    virtual void move(QPoint offset) override;    
    virtual void moveTop(QPoint pos) override;
    virtual void moveBottom(QPoint pos) override;
    virtual void moveLeft(QPoint pos) override;
    virtual void moveRight(QPoint pos) override;
    virtual void moveTopLeft(QPoint pos) override;
    virtual void moveTopRight(QPoint pos) override;
    virtual void moveBottomLeft(QPoint pos) override;
    virtual void moveBottomRight(QPoint pos) override;
    void updateBounds();
    void addPoint(const QPoint& point);  // 添加一个新的点到列表中

    void updateBounds(const QPoint& newPoint);
    

private:
    QVector<QPoint> m_points;  // 用于存储线条上的点
};


#endif //__K_PEN_H_
