#ifndef __K_GLOBAL_DATA__
#define __K_GLOBAL_DATA__

#include <QObject>
#include <QPainter>
class KGlobalData : public QObject
{
	Q_OBJECT

public:
	enum class KDrawFlag
	{
		NoneDrawFlag = 0,
		MouseDrawFlag,	
		PenDrawFlag,
		LineDrawFlag,
		RectDrawFlag,
		CircleDrawFlag,
		TextDrawFlag,
		PentagonDrawFlag,
		PentagramDrawFlag,
		HexagonDrawFlag
	};

	KGlobalData(QObject *parent = Q_NULLPTR);
	~KGlobalData();

	static KGlobalData *getGlobalDataIntance();

	void setDrawFlag(KGlobalData::KDrawFlag drawflag);
	KDrawFlag getDrawFlag();

	void setCanvasWidth(const int width);
	void setCanvasHeight(const int height);
	void setBorderWidth(const int width);

	int getCanvasWidth() const;
	int getBorderWidth() const;
	int getCanvasHeight() const;

	void setCanvasColor(const QString & colorStr);
	void setShapeBorderColor(const QString& colorStr);
	void setShapeFillColor(const QString& colorStr);
	QString getCanvasColor();
	QString getShapeBorderColor();
	QString getShapeFillColor();

	void setCanvasScale(qreal scale);
	qreal getCanvasScale();

	void setBorderStyle(Qt::PenStyle style);
	Qt::PenStyle getBorderStyle();

private:
	KGlobalData(const KGlobalData &other) = delete;
	KGlobalData(const KGlobalData &&other) = delete;
	void operator=(const KGlobalData) = delete;

	KDrawFlag m_drawFlag;
	
	int m_canvasWidth;
	int m_borderWidth;
	int m_canvasHeight;	
	QString m_canvasColor;
	QString m_shapeBorderColor;
	QString m_shapeFillColor;
	
	qreal m_scale;
	Qt::PenStyle m_borderStyle;
};


#endif
