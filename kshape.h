#ifndef __K_SHAPE_H_
#define __K_SHAPE_H_

#include <QObject>
#include <QPoint>
#include <QPaintDevice>
#include <QPainter>

enum class KShapeType
{
	None = 0,
	PenShapeType,
	LineShapeType,
	CircleShapeType,
	RectShapeType,
	PentgaonShapeType,
	PentagramShapeType,
	HexaShapeType,
	TextShapeType,
};

class KShape : public QObject
{
	Q_OBJECT

public:
	KShape(QObject *parent = Q_NULLPTR);
	KShape(const KShape& other);
	~KShape();
	
	// 在子类中重写，实现不同类型的图形的绘制
	virtual void drawShape(QPaintDevice *parent = Q_NULLPTR) = 0;
	virtual KShape* clone() = 0;
	virtual void move(QPoint offset);
	virtual void moveTop(QPoint pos);
	virtual void moveBottom(QPoint pos);
	virtual void moveLeft(QPoint pos);
	virtual void moveRight(QPoint pos);
	virtual void moveTopLeft(QPoint pos);
	virtual void moveTopRight(QPoint pos);
	virtual void moveBottomLeft(QPoint pos);
	virtual void moveBottomRight(QPoint pos);
	virtual KShapeType getShapeType();
	

	virtual QPoint getStartPoint();
	virtual QPoint getEndPoint();


	virtual void setStartPoint(const QPoint &point);
	virtual void setEndPoint(const QPoint &point);
	

	void drawOutLine(QPaintDevice* parent = Q_NULLPTR);
	virtual QRect getShapeRect() const;
	void updateParams();
	bool isValid();

	virtual void resetStartPoint(QPoint point);

	void scalePoint();

protected:
	QPoint m_startPoint;// 起始坐标
	QPoint m_endPoint; // 结束坐标
	Qt::PenStyle m_penStyle = Qt::SolidLine;
	QString m_borderColor = "000000";
	QString m_fillColor = "FFFFFF";
	int m_borderWidth = 2;
	//double m_scaleFactor = 1.0;
};

#endif
