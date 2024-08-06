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
	, m_svgRenderer(nullptr) // 初始化指针
	, m_edit(nullptr)
	, m_pTextShape(nullptr)
{
	m_timer.start(500); // 每0.5秒触发一次
	setAttribute(Qt::WA_StyledBackground, true);

	// 设置背景颜色为白色
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("FFFFFF");
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//设置鼠标追踪
	setFocusPolicy(Qt::ClickFocus);// 单击获取焦点
	(void)connect(&m_timer, &QTimer::timeout, this, &KCanvas::handleTimeout);	
}

KCanvas::~KCanvas()
{
	for (auto shape : m_pShapeList)
		delete shape;
	m_pShapeList.clear();	
	deleteList(m_pCurrentCutShapeList);
	delete m_edit;  // 确保编辑器被正确删除
	//deleteList(m_pCurrentCutShapeList);
}

// 绘图事件处理
void KCanvas::paintEvent(QPaintEvent* event)
{	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// 应用缩放
	//painter.scale(m_scaleFactor, m_scaleFactor);

	// 绘制坐标轴
	//drawAxes(&painter);


	// 如果有SVG渲染器，则使用它进行渲染
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
		if (m_isDrawing) // 图形绘制标志为 true，则表示正在绘制该图形，此时需要不断重新绘制
			m_pCurrentShape->drawShape(this);

		if (m_isSelected) // 图形选中，则绘制选中框
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
	//获取当前需要绘制的图形类型
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	
	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;// 记录鼠标左键已经点击
		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag)
		{
			m_pCurrentShape = getCurrentShape(rescalePoint(event->pos()));
			m_pCurrentSelectedShapeList.clear();
			m_pCurrentSelectedShapeList.prepend(m_pCurrentShape);
			if (m_pCurrentShape != Q_NULLPTR)
			{				
				m_lastPos = rescalePoint(event->pos());// 当前位置移动的最后位置(会更新)
				m_isSelected = true;// 设置选中标志
				m_TransType = getTransType(rescalePoint(event->pos()));// 获取移动类型
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
	updateCusorStyle(flag, transType);//选中时设置鼠标选中样式

	if (event->buttons() != Qt::LeftButton)//bug : 在不点击鼠标左键并在在选中状态，移动鼠标会移动图形
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
		dragMoveShape(transType, rescalePoint(event->pos()));//移动选中图形
	}
	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	if (m_isLPress) // 鼠标左键按下后释放，则表示移动或者绘图完毕
	{
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)// 如果是绘图
		{
			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (m_pCurrentShape->isValid())// 判断图形矩形是否有效
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
					QFont font("Arial", 16);  // 选择 Arial 字体，大小为 16
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

	// 获取图形矩形
	m_pCurrentSelectedShape = m_pCurrentShape;
	QRect rect = m_pCurrentShape->getShapeRect();

	if (!rect.contains(pos))
		return KTransType::None;
	
	qDebug() << "rect = " << rect.topLeft().x();
	qDebug() << "rect = " << rect.topLeft().y();

	// 判断是整体移动，局部拖拽移动
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
	// 如果已经选中
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO：变换图标,设置不同的鼠标样式,根据点击位置，设置不同的鼠标样式
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
		在移动过程中，分为两种移动方式
			(1) 整体移动  contains
			(2) 基于某一个方向的拖拽移动

	*/
	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// 如果是左上角，则当前位置为新的矩形起始位置
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// 如果是向上移动，则修改起始位置的 y 坐标
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // 整体移动
	{
		// m_lastPos 为选中时光标位置，在移动过程中不断赋值为前一次的位置
		QPoint newpos = pos -  m_lastPos; // 计算要移动的偏移
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

	update();  // 触发重绘
}

void KCanvas::zoomOut(double scale) {

	qDebug() << "CanvasScale : " << KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_scaleFactor = scale;
	update();  // 触发重绘
}

void KCanvas::drawAxes(QPainter* painter) {
	int axisLength = 1000;  // 坐标轴的长度
	int tickStep = 100;  // 主刻度间隔

	// 根据缩放调整坐标轴长度和刻度间隔
	int scaledAxisLength = static_cast<int>(axisLength * 1);
	int scaledTickStep = static_cast<int>(tickStep * 1);

	// 绘制X轴
	painter->drawLine(0, 0, scaledAxisLength, 0);
	for (int i = 0; i <= scaledAxisLength; i += scaledTickStep) 
	{
		painter->drawLine(i, -10, i, 10);  // 刻度
	}

	// 绘制Y轴
	painter->drawLine(0, 0, 0, scaledAxisLength);
	for (int i = 0; i <= scaledAxisLength; i += scaledTickStep) 
	{
		painter->drawLine(-10, i, 10, i);  // 刻度
	}

	// 可以添加文本标签等...
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
	update(); // 更新画布
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

	m_localContextMenuPos = rescalePoint(event->pos());  // 保存局部坐标
	qDebug() << "contextMenuEvent" << m_localContextMenuPos;
	QMenu contextMenu(this);
	contextMenu.setStyleSheet("QMenu { background-color: #cccccc; color: black; }"
		"QMenu::item { background-color: transparent; }"
		"QMenu::item:selected { background-color: #6699cc; color: white; }");

	// 全选
	QAction* selectAllAction = new QAction(QString::fromLocal8Bit("全选"), &contextMenu);
	selectAllAction->setShortcut(QKeySequence::SelectAll);
	selectAllAction->setShortcutVisibleInContextMenu(true); // 显示快捷键在上下文菜单中
	connect(selectAllAction, &QAction::triggered, this, &KCanvas::selectAllShapes);
	contextMenu.addAction(selectAllAction);

	// 剪切
	QAction* cutAction = new QAction(QString::fromLocal8Bit("剪切"), &contextMenu);
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setShortcutVisibleInContextMenu(true);
	connect(cutAction, &QAction::triggered, this, &KCanvas::cutShape);
	contextMenu.addAction(cutAction);

	// 复制
	QAction* copyAction = new QAction(QString::fromLocal8Bit("复制"), &contextMenu);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setShortcutVisibleInContextMenu(true);
	connect(copyAction, &QAction::triggered, this, &KCanvas::copyShape);
	contextMenu.addAction(copyAction);

	// 粘贴
	QAction* pasteAction = new QAction(QString::fromLocal8Bit("粘贴"), &contextMenu);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setShortcutVisibleInContextMenu(true);
	connect(pasteAction, &QAction::triggered, this, &KCanvas::pasteShape);
	contextMenu.addAction(pasteAction);

	// 置上一层
	QAction* toTopAction = new QAction(QString::fromLocal8Bit("置上一层"), &contextMenu);
	// 假设您为这个动作设置了自定义快捷键
	toTopAction->setShortcut(QKeySequence("Ctrl+Up"));
	toTopAction->setShortcutVisibleInContextMenu(true);
	connect(toTopAction, &QAction::triggered, this, &KCanvas::toTopActionShape);
	contextMenu.addAction(toTopAction);

	// 置下一层
	QAction* toBottonAction = new QAction(QString::fromLocal8Bit("置下一层"), &contextMenu);
	// 假设您为这个动作设置了自定义快捷键
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
	// 实现全选逻辑
}

void KCanvas::cutShape()
{

	if (m_pCurrentSelectedShapeList.isEmpty()) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("未选中任何图形"));
		return;
	}
	//m_pCurrentShape = getCurrentShape(m_localContextMenuPos);

	m_pCurrentCutShapeList.clear();//清空暂存数组
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();
	m_pCurrentCopyShapeList.clear();//清空暂存数组
	m_pCurrentCopyShapeOffsetList.clear();

	QPoint topLeftPoint = getTopLeftPoint(m_pCurrentSelectedShapeList);
	for (auto& p : m_pCurrentSelectedShapeList)
	{		
		m_pCurrentCutShapeList.prepend(p);// 记录被剪切图形
		m_pCurrentCutShapeOffsetList.prepend(topLeftPoint); //记录相对位置差
		m_pShapeList.removeAll(p);
	}	

	m_isSelected = false;
	m_pCurrentSelectedShapeList.clear();
	update();
	// 实现剪切逻辑
}




void KCanvas::copyShape()
{
	// 实现复制逻辑
	if (m_pCurrentSelectedShapeList.isEmpty()) 
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("未选中任何图形"));
		return;
	}

	m_pCurrentCutShapeList.clear();//清空暂存数组
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();
	m_pCurrentCopyShapeList.clear();//清空暂存数组
	m_pCurrentCopyShapeOffsetList.clear();

	QPoint topLeftPoint = getTopLeftPoint(m_pCurrentSelectedShapeList);
	for (auto& p : m_pCurrentSelectedShapeList)
	{
		m_pCurrentCopyShapeList.prepend(p);// 复制的图形
		m_pCurrentCopyShapeOffsetList.prepend(topLeftPoint); //记录相对位置差
		qDebug() << "Copy" << m_localContextMenuPos;
//		m_pShapeList.removeAll(p);不需要去除
	}

	m_isSelected = false;
	m_pCurrentSelectedShapeList.clear();
	update();
}

void KCanvas::pasteShape()
{// 实现粘贴逻辑
	qDebug() << "paste" << m_localContextMenuPos;
	for (int i = 0; i < m_pCurrentCutShapeList.size(); ++i)
	{
		KShape* p = m_pCurrentCutShapeList[i];
		QPoint offset = m_localContextMenuPos - m_pCurrentCutShapeOffsetList[i];
		p->resetStartPoint(p->getStartPoint() + offset);
		m_pShapeList.prepend(p);// 加入m_pShapeList
	}	
	m_pCurrentCutShapeList.clear();//清空剪切数组
	deleteList(m_pCurrentCutShapeList);
	m_pCurrentCutShapeOffsetList.clear();

	for (int i = 0; i < m_pCurrentCopyShapeList.size(); ++i)
	{
		KShape* p = m_pCurrentCopyShapeList[i]->clone();//克隆一个新的对象
		QPoint offset = m_localContextMenuPos - m_pCurrentCopyShapeOffsetList[i];
		p->resetStartPoint(p->getStartPoint() + offset);
		m_pShapeList.prepend(p);// 加入m_pShapeList
	}
	update();
	m_localContextMenuPos = m_localContextMenuPos + QPoint(50,50);
}

void KCanvas::toTopActionShape()
{
	if (m_pCurrentShape == Q_NULLPTR) return;
	int index = m_pShapeList.indexOf(m_pCurrentShape);  // 找到元素的索引
	if (index != -1) 
	{  // 确保元素存在于列表中
		m_pShapeList.removeAt(index);  // 移除元素
		m_pShapeList.prepend(m_pCurrentShape);   // 在列表首位插入元素
	}
}

void KCanvas::toBottonActionShape()
{
	if (m_pCurrentShape == Q_NULLPTR) return;
	int index = m_pShapeList.indexOf(m_pCurrentShape);  // 找到元素的索引
	if (index != -1) 
	{  // 确保元素存在于列表中
		m_pShapeList.removeAt(index);  // 移除元素
		m_pShapeList.push_back(m_pCurrentShape);   // 在列表末尾插入元素
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
	QFont font("Arial", 16);  // 选择 Arial 字体，大小为 16
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

