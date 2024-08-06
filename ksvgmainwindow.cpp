#include "ksvgmainwindow.h"
#include <QApplication>
#include <QWheelEvent>
#include <QDebug>
KSvgMainWindow::KSvgMainWindow(QWidget *parent)
	: QScrollArea(parent)
	, m_pCanvas(Q_NULLPTR)
{
	setFrameShape(QFrame::NoFrame); // 设置无边框

	// 当内容超过显示区域时出现滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setStyleSheet("QScrollArea{background-color:#3f3f3c;}");
	setAlignment(Qt::AlignCenter);
	
	m_pCanvas = new KCanvas;
	setWidget(m_pCanvas);// 设置当前控件为滚动区域的子控件



}

KSvgMainWindow::~KSvgMainWindow()
{
}



void KSvgMainWindow::setCustorStyle(KGlobalData::KDrawFlag drawFlag)
{
	switch (drawFlag)
	{
	case KGlobalData::KDrawFlag::PenDrawFlag:
		setCursor(Qt::ArrowCursor);
		break;
	case KGlobalData::KDrawFlag::PentagonDrawFlag:
	case KGlobalData::KDrawFlag::PentagramDrawFlag:
	case KGlobalData::KDrawFlag::HexagonDrawFlag:
	case KGlobalData::KDrawFlag::LineDrawFlag:
	case KGlobalData::KDrawFlag::RectDrawFlag:
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		setCursor(Qt::CrossCursor);
		break;

	case KGlobalData::KDrawFlag::TextDrawFlag:
		setCursor(Qt::IBeamCursor);
		break;
	default:
		setCursor(Qt::ArrowCursor);
		break;
	}
}

void KSvgMainWindow::enterEvent(QEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	setCustorStyle(flag);
	QScrollArea::enterEvent(event);
}

void KSvgMainWindow::wheelEvent(QWheelEvent* event)
{
	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		qreal scale = 1.0;
		if (event->delta() > 0)
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() + 0.05;
		else
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() - 0.05;

		qint32 width = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
		qint32 height = KGlobalData::getGlobalDataIntance()->getCanvasHeight();
		m_pCanvas->resize(width * scale, height * scale);
		KGlobalData::getGlobalDataIntance()->setCanvasScale(scale);
		emit updateScale(scale);
		event->accept();  // 标记事件为已处理
	}

}

void KSvgMainWindow::keyPressEvent(QKeyEvent* event) 
{
	if (!m_pCanvas) 
	{
		QScrollArea::keyPressEvent(event);
		return;
	}

	// 检查是否按下了控制键（Ctrl）
	if (event->modifiers() == Qt::ControlModifier) 
	{
		switch (event->key()) 
		{
		case Qt::Key_A:
			m_pCanvas->selectAllShapes();
			event->accept();
			break;
		case Qt::Key_C:
			m_pCanvas->copyShape();
			event->accept();
			break;
		case Qt::Key_V:
			m_pCanvas->pasteShape();
			event->accept();
			break;
		case Qt::Key_X:
			m_pCanvas->cutShape();
			event->accept();
			break;
		case Qt::Key_Up:
			m_pCanvas->toTopActionShape();
			event->accept();
			break;
		case Qt::Key_Down:
			m_pCanvas->toBottonActionShape();
			event->accept();
			break;
		default:
			QScrollArea::keyPressEvent(event);
			break;
		}
	}
	else if (event->key() == Qt::Key_Delete) 
	{
		// 处理删除键（不需要Ctrl修饰符）
		m_pCanvas->cutShape();
		event->accept();
	}
	else 
	{
		QScrollArea::keyPressEvent(event);
	}
}
