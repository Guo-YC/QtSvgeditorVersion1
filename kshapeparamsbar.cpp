#include "kshapeparamsbar.h"
#include "kstylebox.h"
#include "kvaluebox.h"
#include "kcolorbox.h"
KShapeParamsBar::KShapeParamsBar(const QString& title, QWidget* parent)
	: KParamsBar(title, parent)
	, m_pBorderWidthBox(Q_NULLPTR)
	, m_pShapeBorderColorBox(Q_NULLPTR)
	, m_pShapeFillColorBox(Q_NULLPTR)
{
	m_pBorderWidthBox = new KValueBox(QStringLiteral("边框宽度"), KGlobalData::getGlobalDataIntance()->getBorderWidth(), this);
	m_pShapeBorderColorBox = new KColorBox(QStringLiteral("边框颜色"), KGlobalData::getGlobalDataIntance()->getShapeBorderColor(), KColorType::ShapeBorderColorType, this);
	m_pShapeFillColorBox = new KColorBox(QStringLiteral("填充颜色"), KGlobalData::getGlobalDataIntance()->getShapeFillColor(), KColorType::ShapeFillColorType , this);
	m_pShapeBorderStyleBox = new KStyleBox(QStringLiteral("边框样式"), KGlobalData::getGlobalDataIntance()->getBorderStyle(), this);
	// 添加到网格布局
	m_pGridLayout->addWidget(m_pBorderWidthBox, 0, 0);
	m_pGridLayout->addWidget(m_pShapeBorderColorBox, 0, 1);
	m_pGridLayout->addWidget(m_pShapeFillColorBox, 1, 0);
	m_pGridLayout->addWidget(m_pShapeBorderStyleBox, 1, 1);

}

KShapeParamsBar::~KShapeParamsBar()
{
	delete m_pBorderWidthBox;
	delete m_pShapeBorderColorBox;
	delete m_pShapeFillColorBox;
	delete m_pShapeBorderStyleBox;
}

