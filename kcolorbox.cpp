#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,const QString &rgbColorStr, KColorType type, QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
	, m_pColorType(type)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // ���ӵ�������

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

	// QColorDialog : ϵͳʵ����ɫѡ���
	QColor color = QColorDialog::getColor(QColor(str.toUInt(NULL, 16)), nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	str = QString::number(color.rgb(), 16); // �� 16���Ƶ���ɫֵת���� str �ַ���

	// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
	if (m_pColorType == KColorType::CanvasColorType)
		KGlobalData::getGlobalDataIntance()->setCanvasColor(str);
	else if (m_pColorType == KColorType::ShapeBorderColorType)
		KGlobalData::getGlobalDataIntance()->setShapeBorderColor(str);
	else if (m_pColorType == KColorType::ShapeFillColorType)
		KGlobalData::getGlobalDataIntance()->setShapeFillColor(str);

	// ���ð�ť������ɫΪ��ǰѡ�����ɫ
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(str));

	emit pickedColor(); // �����ź�	
}
