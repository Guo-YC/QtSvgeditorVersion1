#ifndef __K_CANVAS_H_
#define __K_CANVAS_H_


#include "kshape.h"
#include "kglobaldata.h"

#include <QWidget>
#include <QTimer>
enum class KTransType
{
	None = 0,
	TopLeft, 
	Top,
	TopRight,
	Left,
	Contains,// 全部 
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};
class QTimer;
class QSvgRenderer;
class QLineEdit;
class KText;
class KCanvas : public QWidget
{
	Q_OBJECT

public:
	KCanvas(QWidget *parent = Q_NULLPTR);
	~KCanvas();
	KCanvas(const KCanvas&) = delete;
	KCanvas& operator=(const KCanvas&) = delete;

	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

	void hideEditor();
	
	KShape* getCurrentShape(const QPoint &pos);;// 获取当前位置图形
	KTransType getTransType(const QPoint &pos); // 获取移动类型
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType,const QPoint &pos);

	void handleTimeout();

	void zoomIn(double scale);

	void zoomOut(double scale);

	void drawAxes(QPainter* painter);

	QPoint rescalePoint(QPoint point);

	QPoint scalePoint(QPoint point);

	void setRenderer(QSvgRenderer* renderer); // 添加setRenderer方法

	void clear();

	void contextMenuEvent(QContextMenuEvent* event);

	void selectAllShapes();

	void cutShape();

	QPoint getTopLeftPoint(QList<KShape*> list);

	void copyShape();

	void pasteShape();

	void toTopActionShape();

	void toBottonActionShape();	

	void deleteList(QList<KShape*> list);

private:
	KShape* m_pCurrentShape;
	KShape* m_pCurrentSelectedShape;
	QList <KShape*> m_pCurrentSelectedShapeList;// 当前被选中的图形
	QList <KShape*> m_pCurrentCutShapeList;// 当前剪切的图形
	QList <QPoint> m_pCurrentCutShapeOffsetList;// 当前剪切的图形offset
	QList <KShape*> m_pCurrentCopyShapeList;// 当前复制的图形
	QList <QPoint> m_pCurrentCopyShapeOffsetList;// 当前复制的图形offset
	QList <KShape*> m_pShapeList;// 存储当前画布中的图形
	
	QPoint m_lastPos;// 记录前一次的位置
	QPoint m_localContextMenuPos;
	KTransType m_TransType;// 记录移动类型
	
	bool m_isLPress;// 标记鼠标左键
	bool m_isDrawing;// 是否绘图
	bool m_isSelected;// 是否为选中
	double m_scaleFactor = 1.0;
	QTimer m_timer;

	QSvgRenderer* m_svgRenderer; // 添加指向QSvgRenderer的指针
	QLineEdit* m_edit;  // 用于编辑文本的 QLineEdit
	KText* m_pTextShape;
};

#endif
