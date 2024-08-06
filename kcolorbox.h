#ifndef __K_PARAMBOX_H_
#define __K_PARAMBOX_H_

#include <QPushButton>
#include <QColorDialog>

#include "kparambox.h"

enum class KColorType
{
	None = 0,
	CanvasColorType,
	ShapeBorderColorType,
	ShapeFillColorType,
};

class KColorBox : public KParamBox
{
	Q_OBJECT

public:
	KColorBox(const QString &title,const QString &rgbColorStr, KColorType type,
		QWidget *parent);
	~KColorBox();

	// ����ͷ��¼�,����ͷ�ʱ������ɫѡ��Ի���
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
signals:
	void pickedColor();
private slots:
	void showPickColorDlg();
private:
	QPushButton* m_pColorBtn;
	KColorType m_pColorType;
};

#endif