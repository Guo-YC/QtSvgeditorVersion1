#include "kcanvas.h"
#include "ktext.h"
#include "kshapefactory.h"
#include "kglobaldata.h"
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QMenu>
#include <QLineEdit>
#include <typeinfo>
#include <QSvgRenderer>
#include <QMessageBox>
KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_pCurrentShape(Q_NULLPTR)
	, m_pCurrentSelectedShape(Q_NULLPTR)
	, m_TransType(KTransType::None)
	, m_isDrawing(false)
	, m_isLPress(false)
	, m_isSelected(false)
	, m_svgRenderer(nullptr) // ��ʼ��ָ��
	, m_edit(nullptr)
	, m_pTextShape(nullptr)
{
	m_timer.start(500); // ÿ0.5�봥��һ��
	setAttribute(Qt::WA_StyledBackground, true);

	// ���ñ�����ɫΪ��ɫ
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("FFFFFF");
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//�������׷��
	setFocusPolicy(Qt::ClickFocus);// ������ȡ����
	(void)connect(&m_timer, &QTimer::timeout, this, &KCanvas::handleTimeout);	
}

KCanvas::~KCanvas()
{
	for (auto shape : m_pShapeList)
		delete shape;
	m_pShapeList.clear();	
	deleteList(m_pCurrentCutShapeList);
	delete m_edit;  // ȷ���༭������ȷɾ��
	//deleteList(m_pCurrentCutShapeList);
}

// ��ͼ�¼�����
void KCanvas::paintEvent(QPaintEvent* event)
{	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// Ӧ������
	//painter.scale(m_scaleFactor, m_scaleFactor);

	// ����������
	//drawAxes(&painter);


	// �����SVG��Ⱦ������ʹ����������Ⱦ
	if (m_svgRenderer && m_svgRenderer->isValid()) 
	{
		m_svgRenderer->render(&painter);
	}

	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawShape(this);
			it++;
		}	
	}

	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_isDrawing) // ͼ�λ��Ʊ�־Ϊ true�����ʾ���ڻ��Ƹ�ͼ�Σ���ʱ��Ҫ�������»���
			m_pCurrentShape->drawShape(this);

		if (m_isSelected) // ͼ��ѡ�У������ѡ�п�
			m_pCurrentShape->drawOutLine(this);
		
	}

	if (!m_pCurrentSelectedShapeList.isEmpty())
	{
		if (m_isSelected)
		{
			for (QList<KShape*>::iterator it = m_pCurrentSelectedShapeList.begin();
				it != m_pCurrentSelectedShapeList.end(); ++it)
			{
				if(m_pShapeList.contains(*it) )
					(*it)->drawOutLine(this);
			}
		}

	}
}

void KCanvas::mousePressEvent(QMouseEvent* event)
{
	//��ȡ��ǰ��Ҫ���Ƶ�ͼ������
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	
	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;// ��¼�������Ѿ����
		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag)
		{
			m_pCurrentShape = getCurrentShape(rescalePoint(event->pos()));
			m_pCurrentSelectedShapeList.clear();
			m_pCurrentSelectedShapeList.prepend(m_pCurrentShape);
			if (m_pCurrentShape != Q_NULLPTR)
			{				
				m_lastPos = rescalePoint(event->pos());// ��ǰλ���ƶ������λ��(�����)
				m_isSelected = true;// ����ѡ�б�־
				m_TransType = getTransType(rescalePoint(event->pos()));// ��ȡ�ƶ�����
			}
			else
			{
				m_isSelected = false;
				m_pCurrentSelectedShapeList.clear();
			}
		}
		else
		{ 
			m_isSelected = false;
			m_pCurrentSelectedShapeList.clear();
			m_pCurrentShape = KShapeFactory::createShape(KGlobalData::getGlobalDataIntance()->getDrawFlag());			
			if (m_pCurrentShape != Q_NULLPTR)
			{
				m_pCurrentShape->updateParams();
				m_pCurrentShape->setStartPoint(rescalePoint(event->pos()));
			}
				
				
		}
	}
	update();
}

void KCanvas::mouseMoveEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;


	KTransType transType = getTransType(rescalePoint(event->pos()));
	updateCusorStyle(flag, transType);//ѡ��ʱ�������ѡ����ʽ

	if (event->buttons() != Qt::LeftButton)//bug : �ڲ����������������ѡ��״̬���ƶ������ƶ�ͼ��
			return;


	if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		if (m_isLPress && !m_isDrawing)
			m_isDrawing = true;
		if (m_pCurrentShape != Q_NULLPTR)
			m_pCurrentShape->setEndPoint(rescalePoint(event->pos()));
	}
	else
	{
		if (m_pCurrentShape == Q_NULLPTR)
			return;
		dragMoveShape(transType, rescalePoint(event->pos()));//�ƶ�ѡ��ͼ��
	}
	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	if (m_isLPress) // ���������º��ͷţ����ʾ�ƶ����߻�ͼ���
	{
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)// ����ǻ�ͼ
		{
			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (m_pCurrentShape->isValid())// �ж�ͼ�ξ����Ƿ���Ч
					m_pCurrentShape->setEndPoint(rescalePoint(event->pos()));

				if (KGlobalData::getGlobalDataIntance()->getDrawFlag() == KGlobalData::KDrawFlag::TextDrawFlag)
				{
					if (m_edit) {
						m_edit->deleteLater();
						m_edit = nullptr;
					}
					m_pCurrentShape->setEndPoint(rescalePoint(event->pos()+QPoint(150, 50)));
					m_pTextShape = static_cast<KText*> (m_pCurrentShape);								
					m_edit = new QLineEdit(this);
					QRect rect(m_pTextShape->getStartPoint(), m_pTextShape->getEndPoint());
					m_edit->setGeometry(rect);
					m_edit->setText(m_pTextShape->getText());
					QFont font("Arial", 16);  // ѡ�� Arial ���壬��СΪ 16
					m_edit->selectAll();
					m_edit->setFont(font);
					m_edit->setFocus();
					m_edit->show();
					(void)connect(m_edit, &QLineEdit::editingFinished, this, &KCanvas::hideEditor);
				}
				m_pShapeList.prepend(m_pCurrentShape);
				m_pCurrentShape = Q_NULLPTR;
				//m_isSelected = true;
				//m_isDrawing = false;
					
			}
			else
			{
				delete m_pCurrentShape;
				m_pCurrentShape = Q_NULLPTR;
			}
		}

		m_isLPress = false;
		m_isDrawing = false;
		m_TransType = KTransType::None;
	}

	update();
}

KShape* KCanvas::getCurrentShape(const QPoint& pos)
{
	KShape* res = nullptr;
	long long area = LLONG_MAX;
	for (QList<KShape*>::iterator it = m_pShapeList.begin();
		it != m_pShapeList.end(); ++it)
	{
		if ((*it)->getShapeRect().contains(pos))
		{
			long long curArea = abs((long long)(*it)->getStartPoint().rx() - (long long)(*it)->getEndPoint().rx())
				* abs((long long)(*it)->getStartPoint().ry() - (long long)(*it)->getEndPoint().ry());
			if (curArea < area)
			{
				res = *it;
				area = curArea;
			}
		}
	}
	if (res!=nullptr)
		res->updateParams();
	return res;
}

KTransType KCanvas::getTransType(const QPoint& pos)
{
	if (!m_pCurrentShape)
		return KTransType::None;

	// ��ȡͼ�ξ���
	m_pCurrentSelectedShape = m_pCurrentShape;
	QRect rect = m_pCurrentShape->getShapeRect();

	if (!rect.contains(pos))
		return KTransType::None;
	
	qDebug() << "rect = " << rect.topLeft().x();
	qDebug() << "rect = " << rect.topLeft().y();

	// �ж��������ƶ����ֲ���ק�ƶ�
	if (qAbs(pos.x() - rect.topLeft().x()) < 15 
			&& qAbs(pos.y() - rect.topLeft().y()) < 5)
		return KTransType::TopLeft;

	if (qAbs(rect.topRight().x() - pos.x()) < 15 
			&& qAbs(pos.y() - rect.topRight().y()) < 15)
		return KTransType::TopRight;

	if (qAbs(rect.bottomRight().x() - pos.x()) < 15 
			&& qAbs(rect.bottomRight().y() - pos.y()) < 15)
		return KTransType::BottomRight;

	if (qAbs(pos.x() - rect.bottomLeft().x()) < 15 
			&& qAbs(rect.bottomLeft().y() - pos.y()) < 15)
		return KTransType::BottomLeft;

	if (qAbs(pos.y() - rect.top()) < 15 
			&& pos.x() > rect.topLeft().x() + 15
			&& pos.x() < rect.topRight().x() - 15)
		return KTransType::Top;

	if (qAbs(rect.right() - pos.x()) < 15
			&& pos.y() > rect.topRight().y() + 15
			&& pos.y() < rect.bottomRight().y() - 15)
		return KTransType::Right;

	if (qAbs(rect.bottom() - pos.y()) < 15 
			&& pos.x() > rect.bottomLeft().x() + 15 
			&& pos.x() < rect.bottomRight().x() - 15)
		return KTransType::Bottom;

	if (qAbs(pos.x() - rect.left()) < 15 
			&& pos.y() > rect.topLeft().y() + 15 
			&& pos.y() < rect.bottomLeft().y() - 15)
		return KTransType::Left;

	return KTransType::Contains;
}

void KCanvas::updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType)
{
	// ����Ѿ�ѡ��
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO���任ͼ��,���ò�ͬ�������ʽ,���ݵ��λ�ã����ò�ͬ�������ʽ
		if (transType == KTransType::TopLeft || transType == KTransType::BottomRight)
			setCursor(Qt::SizeFDiagCursor);
		else if (transType == KTransType::TopRight || transType == KTransType::BottomLeft)
			setCursor(Qt::SizeBDiagCursor);
		else if (transType == KTransType::Top || transType == KTransType::Bottom)
			setCursor(Qt::SizeVerCursor);
		else if (transType == KTransType::Left || transType == KTransType::Right)
			setCursor(Qt::SizeHorCursor);

		else if (transType == KTransType::Contains)
			setCursor(Qt::SizeAllCursor);
	}
	else
		unsetCursor();
}

void KCanvas::dragMoveShape(KTransType transType,const QPoint &pos)
{
	if (m_pCurrentShape == NULL)
		return;

	/*
		���ƶ������У���Ϊ�����ƶ���ʽ
			(1) �����ƶ�  contains
			(2) ����ĳһ���������ק�ƶ�

	*/
	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// ��������Ͻǣ���ǰλ��Ϊ�µľ�����ʼλ��
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// ����������ƶ������޸���ʼλ�õ� y ����
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // �����ƶ�
	{
		// m_lastPos Ϊѡ��ʱ���λ�ã����ƶ������в��ϸ�ֵΪǰһ�ε�λ��
		QPoint newpos = pos -  m_lastPos; // ����Ҫ�ƶ���ƫ��
		//m_pCurrentShape->move(newpos);
		

		for (auto p : m_pCurrentSelectedShapeList)
		{
			p->move(newpos);
		}
		m_lastPos = pos;
	}
	break;
	case KTransType::Right:
		m_pCurrentShape->moveRight(pos);
		break;
	case KTransType::BottomLeft:
		m_pCurrentShape->moveBottomLeft(pos);
		break;
	case KTransType::Bottom:
		m_pCurrentShape->moveBottom(pos);
		break;
	case KTransType::BottomRight:
		m_pCurrentShape->moveBottomRight(pos);
		break;
	default:
		break;
	}
}


void KCanvas::handleTimeout()
{
	if (m_pCurrentSelectedShape != Q_NULLPTR)
	{
		if (m_isSelected) 
		{
			m_pCurrentSelectedShape->updateParams();
			update();
		}
			
	}	

}

void KCanvas::zoomIn(double scale) {

	m_scaleFactor = scale;
	qDebug() << "CanvasScale : " << KGlobalData::getGlobalDataIntance()->getCanvasScale();

	update();  // �����ػ�
}

void KCanvas::zoomOut(double scale) {

	qDebug() << "CanvasScale : " << KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_scaleFactor = scale;
	update();  // �����ػ�
}

void KCanvas::drawAxes(QPainter* painter) {
	int axisLength = 1000;  // ������ĳ���
	int tickStep = 100;  // ���̶ȼ��

	// �������ŵ��������᳤�ȺͿ̶ȼ��
	int scaledAxisLength = static_cast<int>(axisLength * 1);
	int scaledTickStep = static_cast<int>(tickStep * 1);

	// ����X��
	painter->drawLine(0, 0, scaledAxisLength, 0);
	for (int i = 0; i <= scaledAxisLength; i += scaledTickStep) 
	{
		painter->drawLine(i, -10, i, 10);  // �̶�
	}

	// ����Y��
	painter->drawLine(0, 0, 0, scaledAxisLength);
	for (int i = 0; i <= scaledAxisLength; i += scaledTickStep) 
	{
		painter->drawLine(-10, i, 10, i);  // �̶�
	}

	// ��������ı���ǩ��...
}

QPoint KCanvas::rescalePoint(QPoint point)
{
	point.ry() = point.y() / KGlobalData::getGlobalDataIntance()->getCanvasScale();
	point.rx() = point.x() / KGlobalData::getGlobalDataIntance()->getCanvasScale();
	return point;
}

QPoint KCanvas::scalePoint(QPoint point)
{
	point.ry() = point.y() * KGlobalData::getGlobalDataIntance()->getCanvasScale();
	point.rx() = point.x() * KGlobalData::getGlobalDataIntance()->getCanvasScale();
	return point;
}


void KCanvas::setRenderer(QSvgRenderer* renderer)
{
	m_svgRenderer = renderer;
	update(); // ���»���
}


void KCanvas::clear()
{
	for (auto shape : m_pShapeList)
		delete shape;
	m_pShapeList.clear();
	m_pCurrentShape = Q_NULLPTR;
	m_pCurrentSelectedShape = Q_NULLPTR;
	m_svgRenderer = Q_NULLPTR;
}


void KCanvas::contextMenuEvent(QContextMenuEvent* event)
{

	m_localContextMenuPos = rescalePoint(event->pos());  // ����ֲ�����
	qDebug() << "contextMenuEvent" << m_localContextMenuPos;
	QMenu contextMenu(this);
	contextMenu.setStyleSheet("QMenu { background-color: #cccccc; color: black; }"
		"QMenu::item { background-color: transparent; }"
		"QMenu::item:selected { background-color: #6699cc; color: white; }");

	// ȫѡ
	QAction* selectAllAction = new QAction(QString::fromLocal8Bit("ȫѡ"), &contextMenu);
	selectAllAction->setShortcut(QKeySequence::SelectAll);
	selectAllAction->setShortcutVisibleInContextMenu(true); // ��ʾ��ݼ��������Ĳ˵���
	connect(selectAllAction, &QAction::triggered, this, &KCanvas::selectAllShapes);
	contextMenu.addAction(selectAllAction);

	// ����
	QAction* cutAction = new QAction(QString::fromLocal8Bit("����"), &contextMenu);
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setShortcutVisibleInContextMenu(true);
	connect(cutAction, &QAction::triggered, this, &KCanvas::cutShape);
	contextMenu.addAction(cutAction);

	// ����
	QAction* copyAction = new QAction(QString::fromLocal8Bit("����"), &contextMenu);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setShortcutVisibleInContextMenu(true);
	connect(copyAction, &QAction::triggered, this, &KCanvas::copyShape);
	contextMenu.addAction(copyAction);

	// ճ��
	QAction* pasteAction = new QAction(QString::fromLocal8Bit("ճ��"), &contextMenu);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setShortcutVisibleInContextMenu(true);
	connect(pasteAction, &QAction::triggered, this, &KCanvas::pasteShape);
	contextMenu.addAction(pasteAction);

	// ����һ��
	QAction* toTopAction = new QAction(QString::fromLocal8Bit("����һ��"), &contextMenu);
	// ������Ϊ��������������Զ����ݼ�
	toTopAction->setShortcut(QKeySequence("Ctrl+Up"));
	toTopAction->setShortcutVisibleInContextMenu(true);
	connect(toTopAction, &QAction::triggered, this, &KCanvas::toTopActionShape);
	contextMenu.addAction(toTopAction);

	// ����һ��
	QAction* toBottonAction = new QAction(QString::fromLocal8Bit("����һ��"), &contextMenu);
	// ������Ϊ��������������Զ����ݼ�
	toBottonAction->setShortcut(QKeySequence("Ctrl+Down"));
	toBottonAction->setShortcutVisibleInContextMenu(true);
	connect(toBottonAction, &QAction::triggered, this, &KCanvas::toBottonActionShape);
	contextMenu.addAction(toBottonAction);


	contextMenu.exec(event->globalPos());
}

void KCanvas::selectAllShapes()
{
	m_pCurrentSelectedShapeList = m_pShapeList;
	m_isSelected = true;
	// ʵ��ȫѡ�߼�
}

void KCanvas::cutShape()
{

	if (m_pCurrentSelectedShapeList.isEmpty()) {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("δѡ���κ�ͼ��"));
		return;
	}
	//m_pCurrentShape = getCurrentShape(m_localContextMenuPos);

	m_pCurrentCutShapeList.clear();//����ݴ�����
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();
	m_pCurrentCopyShapeList.clear();//����ݴ�����
	m_pCurrentCopyShapeOffsetList.clear();

	QPoint topLeftPoint = getTopLeftPoint(m_pCurrentSelectedShapeList);
	for (auto& p : m_pCurrentSelectedShapeList)
	{		
		m_pCurrentCutShapeList.prepend(p);// ��¼������ͼ��
		m_pCurrentCutShapeOffsetList.prepend(topLeftPoint); //��¼���λ�ò�
		m_pShapeList.removeAll(p);
	}	

	m_isSelected = false;
	m_pCurrentSelectedShapeList.clear();
	update();
	// ʵ�ּ����߼�
}




void KCanvas::copyShape()
{
	// ʵ�ָ����߼�
	if (m_pCurrentSelectedShapeList.isEmpty()) 
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("δѡ���κ�ͼ��"));
		return;
	}

	m_pCurrentCutShapeList.clear();//����ݴ�����
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();
	m_pCurrentCopyShapeList.clear();//����ݴ�����
	m_pCurrentCopyShapeOffsetList.clear();

	QPoint topLeftPoint = getTopLeftPoint(m_pCurrentSelectedShapeList);
	for (auto& p : m_pCurrentSelectedShapeList)
	{
		m_pCurrentCopyShapeList.prepend(p);// ���Ƶ�ͼ��
		m_pCurrentCopyShapeOffsetList.prepend(topLeftPoint); //��¼���λ�ò�
		qDebug() << "Copy" << m_localContextMenuPos;
//		m_pShapeList.removeAll(p);����Ҫȥ��
	}

	m_isSelected = false;
	m_pCurrentSelectedShapeList.clear();
	update();
}

void KCanvas::pasteShape()
{// ʵ��ճ���߼�
	qDebug() << "paste" << m_localContextMenuPos;
	for (int i = 0; i < m_pCurrentCutShapeList.size(); ++i)
	{
		KShape* p = m_pCurrentCutShapeList[i];
		QPoint offset = m_localContextMenuPos - m_pCurrentCutShapeOffsetList[i];
		p->resetStartPoint(p->getStartPoint() + offset);
		m_pShapeList.prepend(p);// ����m_pShapeList
	}	
	m_pCurrentCutShapeList.clear();//��ռ�������
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();

	for (int i = 0; i < m_pCurrentCopyShapeList.size(); ++i)
	{
		KShape* p = m_pCurrentCopyShapeList[i]->clone();//��¡һ���µĶ���
		QPoint offset = m_localContextMenuPos - m_pCurrentCopyShapeOffsetList[i];
		p->resetStartPoint(p->getStartPoint() + offset);
		m_pShapeList.prepend(p);// ����m_pShapeList
	}
	update();
	m_localContextMenuPos = m_localContextMenuPos + QPoint(50,50);
}

void KCanvas::toTopActionShape()
{
	if (m_pCurrentShape == Q_NULLPTR) return;
	int index = m_pShapeList.indexOf(m_pCurrentShape);  // �ҵ�Ԫ�ص�����
	if (index != -1) 
	{  // ȷ��Ԫ�ش������б���
		m_pShapeList.removeAt(index);  // �Ƴ�Ԫ��
		m_pShapeList.prepend(m_pCurrentShape);   // ���б���λ����Ԫ��
	}
}

void KCanvas::toBottonActionShape()
{
	if (m_pCurrentShape == Q_NULLPTR) return;
	int index = m_pShapeList.indexOf(m_pCurrentShape);  // �ҵ�Ԫ�ص�����
	if (index != -1) 
	{  // ȷ��Ԫ�ش������б���
		m_pShapeList.removeAt(index);  // �Ƴ�Ԫ��
		m_pShapeList.push_back(m_pCurrentShape);   // ���б�ĩβ����Ԫ��
	}
}

void KCanvas::deleteList(QList<KShape*> list)
{
	for (auto p : list)
	{
		delete(p);
	}
}


QPoint KCanvas::getTopLeftPoint(QList<KShape*> list)
{
	if (list.isEmpty()) return QPoint(0, 0);
	QPoint minPoint = list[0]->getStartPoint();
	for (KShape* shape : list)
	{
		QPoint& point = shape->getStartPoint();
		if (point.x() < minPoint.x()) minPoint.setX(point.x());
		if (point.y() < minPoint.y()) minPoint.setY(point.y());
	}
	return minPoint;
}

void KCanvas::mouseDoubleClickEvent(QMouseEvent* event) {
	if (m_edit) 
	{
		m_edit->deleteLater();
		m_edit = nullptr;
	}

	m_pTextShape=static_cast<KText*> (getCurrentShape(rescalePoint(event->pos())));
	if (!m_pTextShape) return;

	if (m_pTextShape->getShapeType() != KShapeType::TextShapeType) return;

	m_edit = new QLineEdit(this);
	QRect rect(m_pTextShape->getStartPoint(), m_pTextShape->getEndPoint());
	m_edit->setGeometry(rect);
	m_edit->setText(m_pTextShape->getText());
	QFont font("Arial", 16);  // ѡ�� Arial ���壬��СΪ 16
	m_edit->setFont(font);
	m_edit->selectAll();
	m_edit->setFocus();
	m_edit->show();

	(void)connect(m_edit, &QLineEdit::editingFinished, this, &KCanvas::hideEditor);
}

void KCanvas::hideEditor() 
{
	if (m_edit && m_pTextShape) 
	{
				
		if (m_pTextShape) 
		{
			m_pTextShape->setText(m_edit->text());
		}
		m_edit->deleteLater();
		m_edit = nullptr;
	}
	update();
}

