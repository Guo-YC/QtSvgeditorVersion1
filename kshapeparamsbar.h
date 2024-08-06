#ifndef __K_SHAPE_PARAMS_BAR_H_
#define __K_SHAPE_PARAMS_BAR_H_

#include "kparamsbar.h"

class KStyleBox;
class KColorBox;
class KValueBox;
class KShapeParamsBar : public KParamsBar
{
	Q_OBJECT
		friend class KMainWindow;
public:
	KShapeParamsBar(const QString& title, QWidget* parent);
	~KShapeParamsBar();
private:
	KValueBox *m_pBorderWidthBox;
	KColorBox *m_pShapeBorderColorBox;
	KColorBox *m_pShapeFillColorBox;
	KStyleBox *m_pShapeBorderStyleBox;
};

#endif
