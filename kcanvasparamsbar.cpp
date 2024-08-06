#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
	, m_pScaleBox(Q_NULLPTR)
	, m_pBorderWidthBox(Q_NULLPTR)
	, m_pShapeBorderColorBox(Q_NULLPTR)	
	, m_pShapeFillColorBox(Q_NULLPTR)
{
	m_pWidthBox = new KValueBox(QStringLiteral("�������"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("�����߶�"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pColorBox = new KColorBox(QStringLiteral("������ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(),KColorType::CanvasColorType, this);
	m_pScaleBox = new KValueBox(QStringLiteral("�������ű���"), KGlobalData::getGlobalDataIntance()->getCanvasScale()*100, this);
	// ��ӵ����񲼾�
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// �� 0 �� �� 0 ��
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// �� 0 �� �� 1 ��
	m_pGridLayout->addWidget(m_pColorBox, 1, 0);
	m_pGridLayout->addWidget(m_pScaleBox, 1, 1);

}

KCanvasParamsBar::~KCanvasParamsBar()
{
	delete m_pWidthBox;
	delete m_pHeightBox;
	delete m_pColorBox;
	delete m_pScaleBox;
	delete m_pBorderWidthBox;
	delete m_pShapeBorderColorBox;
	delete m_pShapeFillColorBox;
}
