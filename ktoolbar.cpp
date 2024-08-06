#include "ktoolbar.h"
#include "kglobaldata.h"

KToolBar::KToolBar(QWidget *parent)
	: QWidget(parent)
	, m_pVLayout(NULL)
	, m_pBtnGroup(NULL)
	, m_pMouseBtn(NULL)
	, m_pPenBtn(NULL)
	, m_pLineBtn(NULL)
	, m_pRectBtn(NULL)
	, m_pCircleBtn(NULL)
	, m_pTextBtn(NULL)
{
	setMaximumWidth(50);// 设置最大宽度为 50
	setAttribute(Qt::WA_StyledBackground, true);// 自定义控件样式表生效
	setStyleSheet("background-color:#000000");// 设置样式表

	//垂直布局对象
	m_pVLayout = new QVBoxLayout(this);
	m_pVLayout->setAlignment(Qt::AlignVCenter | Qt::AlignTop);// 设置对齐方式
	m_pVLayout->setSpacing(25);// 设置布局器中的组件的间距

	m_pBtnGroup = new QButtonGroup(this);
	m_pBtnGroup->setExclusive(true);// 设置互斥，只能有一个按钮处于 checked 状态

	// 创建自定义按钮
	m_pMouseBtn = new KToolButton(KGlobalData::KDrawFlag::MouseDrawFlag, this);
	m_pPenBtn = new KToolButton(KGlobalData::KDrawFlag::PenDrawFlag, this);
	m_pLineBtn = new KToolButton(KGlobalData::KDrawFlag::LineDrawFlag, this);
	m_pRectBtn = new KToolButton(KGlobalData::KDrawFlag::RectDrawFlag, this);
	m_pCircleBtn = new KToolButton(KGlobalData::KDrawFlag::CircleDrawFlag, this);
	m_pTextBtn = new KToolButton(KGlobalData::KDrawFlag::TextDrawFlag, this);
	m_pPentagonBtn = new KToolButton(KGlobalData::KDrawFlag::PentagonDrawFlag, this);
	m_pPentagramBtn = new KToolButton(KGlobalData::KDrawFlag::PentagramDrawFlag, this);
	m_pHexagonBtn = new KToolButton(KGlobalData::KDrawFlag::HexagonDrawFlag, this);


	// 将按钮添加到 按钮组中	
	m_pBtnGroup->addButton(m_pMouseBtn);
	m_pBtnGroup->addButton(m_pPenBtn);
	m_pBtnGroup->addButton(m_pLineBtn);
	m_pBtnGroup->addButton(m_pRectBtn);
	m_pBtnGroup->addButton(m_pCircleBtn);
	m_pBtnGroup->addButton(m_pTextBtn);
	m_pBtnGroup->addButton(m_pPentagonBtn);
	m_pBtnGroup->addButton(m_pPentagramBtn);
	m_pBtnGroup->addButton(m_pHexagonBtn);


	// 将自定义按钮添加到布局器中	
	m_pVLayout->addWidget(m_pMouseBtn);
	m_pVLayout->addWidget(m_pPenBtn);
	m_pVLayout->addWidget(m_pLineBtn);
	m_pVLayout->addWidget(m_pRectBtn);
	m_pVLayout->addWidget(m_pCircleBtn);
	m_pVLayout->addWidget(m_pTextBtn);
	m_pVLayout->addWidget(m_pPentagonBtn);
	m_pVLayout->addWidget(m_pPentagramBtn);
	m_pVLayout->addWidget(m_pHexagonBtn);

	
	// 将布局器添加到 qwidget 中
	setLayout(m_pVLayout);
}


KToolBar::~KToolBar()
{
}
