#include "kstylebox.h"
#include "kglobaldata.h"
#include "kparambox.h"
#include "kicononlydelegate.h"

#include <QPushButton>
#include <QComboBox>
#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QPainter>
#include <QDebug>
KStyleBox::KStyleBox(const QString& title, const Qt::PenStyle& borderStyleStr, QWidget* parent)
	: KParamBox(title, parent)	
	, m_pStyleBox(Q_NULLPTR)	
{
	m_pStyleBox = new QComboBox(this);
	m_pStyleBox->setItemDelegate(new KIconOnlyDelegate());
	m_pStyleBox->setMaximumSize(70, 50);
	 //添加不同的笔画风格到下拉框	
	m_pStyleBox->addItem(createPenStyleIcon(Qt::SolidLine), "", QVariant(static_cast<int>(Qt::SolidLine)));
	m_pStyleBox->addItem(createPenStyleIcon(Qt::DashLine), "", QVariant(static_cast<int>(Qt::DashLine)));
	m_pStyleBox->addItem(createPenStyleIcon(Qt::DotLine), "", QVariant(static_cast<int>(Qt::DotLine)));

	KParamBox::m_pVLayout->addWidget(m_pStyleBox);
	//(void)connect(m_pStyleBox, &QPushButton::clicked, this, &KStyleBox::showPickColorDlg);
		// 连接信号和槽
	(void)connect(m_pStyleBox, QOverload<int>::of(&QComboBox::activated), this, [=](int index) 
		{
			qDebug() << "index:" << index;
			if (index == 0)
			{
				KGlobalData::getGlobalDataIntance()->setBorderStyle(Qt::SolidLine);
			}			
			else if (index == 1)
			{
				KGlobalData::getGlobalDataIntance()->setBorderStyle(Qt::DashLine);
			}		
			else if (index == 2)
			{
				KGlobalData::getGlobalDataIntance()->setBorderStyle(Qt::DotLine);
			}			
		});
}

// 创建笔画风格的图标
QIcon KStyleBox::createPenStyleIcon(Qt::PenStyle style) 
{
	QImage image(50, 30, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	QPen pen(Qt::white);
	pen.setStyle(style);
	pen.setWidth(8);
	painter.setPen(pen);
	painter.drawLine(0, 15, 50,15);
	return QIcon(QPixmap::fromImage(image));
}

KStyleBox::~KStyleBox()
{
	delete m_pStyleBox;
}


