#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,const QString &rgbColorStr, KColorType type, QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
	, m_pColorType(type)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // 添加到布局器

	(void)connect(m_pColorBtn, &QPushButton::clicked, this, &KColorBox::showPickColorDlg);
}

KColorBox::~KColorBox()
{
}

void KColorBox::mouseReleaseEvent(QMouseEvent* event)
{
	m_pColorBtn->click();
}

void KColorBox::showPickColorDlg()
{
	QString str = KGlobalData::getGlobalDataIntance()->getCanvasColor();

	// QColorDialog : 系统实现颜色选择框
	QColor color = QColorDialog::getColor(QColor(str.toUInt(NULL, 16)), nullptr, QStringLiteral("选择颜色"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	str = QString::number(color.rgb(), 16); // 将 16进制的颜色值转换成 str 字符串

	// 将当前颜色值保存到全局数据对象中
	if (m_pColorType == KColorType::CanvasColorType)
		KGlobalData::getGlobalDataIntance()->setCanvasColor(str);
	else if (m_pColorType == KColorType::ShapeBorderColorType)
		KGlobalData::getGlobalDataIntance()->setShapeBorderColor(str);
	else if (m_pColorType == KColorType::ShapeFillColorType)
		KGlobalData::getGlobalDataIntance()->setShapeFillColor(str);

	// 设置按钮背景颜色为当前选择的颜色
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(str));

	emit pickedColor(); // 发出信号	
}

