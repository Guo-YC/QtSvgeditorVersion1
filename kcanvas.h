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
	Contains,// ȫ�� 
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
	
	KShape* getCurrentShape(const QPoint &pos);;// ��ȡ��ǰλ��ͼ��
	KTransType getTransType(const QPoint &pos); // ��ȡ�ƶ�����
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType,const QPoint &pos);

	void handleTimeout();

	void zoomIn(double scale);

	void zoomOut(double scale);

	void drawAxes(QPainter* painter);

	QPoint rescalePoint(QPoint point);

	QPoint scalePoint(QPoint point);

	void setRenderer(QSvgRenderer* renderer); // ���setRenderer����

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
	QList <KShape*> m_pCurrentSelectedShapeList;// ��ǰ��ѡ�е�ͼ��
	QList <KShape*> m_pCurrentCutShapeList;// ��ǰ���е�ͼ��
	QList <QPoint> m_pCurrentCutShapeOffsetList;// ��ǰ���е�ͼ��offset
	QList <KShape*> m_pCurrentCopyShapeList;// ��ǰ���Ƶ�ͼ��
	QList <QPoint> m_pCurrentCopyShapeOffsetList;// ��ǰ���Ƶ�ͼ��offset
	QList <KShape*> m_pShapeList;// �洢��ǰ�����е�ͼ��
	
	QPoint m_lastPos;// ��¼ǰһ�ε�λ��
	QPoint m_localContextMenuPos;
	KTransType m_TransType;// ��¼�ƶ�����
	
	bool m_isLPress;// ���������
	bool m_isDrawing;// �Ƿ��ͼ
	bool m_isSelected;// �Ƿ�Ϊѡ��
	double m_scaleFactor = 1.0;
	QTimer m_timer;

	QSvgRenderer* m_svgRenderer; // ���ָ��QSvgRenderer��ָ��
	QLineEdit* m_edit;  // ���ڱ༭�ı��� QLineEdit
	KText* m_pTextShape;
};

#endif
