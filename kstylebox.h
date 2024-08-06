#ifndef _K_STYLEBOX_H_
#define _K_STYLEBOX_H_

#include "kparambox.h"
#include <QComboBox>
class  QVBoxLayout;
class QIcon;
class QComboBox;
class KStyleBox :public KParamBox
{
	Q_OBJECT
public:
	KStyleBox(const QString& title, const Qt::PenStyle& borderStyleStr, QWidget* parent);	
	~KStyleBox();
	QIcon createPenStyleIcon(Qt::PenStyle style);
protected:
	QVBoxLayout* m_pVLayout;
private:
	QString m_title;
	QComboBox* m_pStyleBox;
};

#endif // _K_STYLE_H_