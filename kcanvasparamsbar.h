#ifndef __K_CANVAS_PARAMS_BAR_H_
#define __K_CANVAS_PARAMS_BAR_H_

#include "kparamsbar.h"
#include "kcolorbox.h"
#include "kvaluebox.h"

class KCanvasParamsBar : public KParamsBar
{
	Q_OBJECT
	friend class KMainWindow;
public:
	KCanvasParamsBar(const QString &title,QWidget *parent);
	~KCanvasParamsBar();
private:
	KValueBox *m_pWidthBox;
	KValueBox *m_pHeightBox;
	KColorBox *m_pColorBox;
	KValueBox *m_pScaleBox;
	KValueBox *m_pBorderWidthBox;
	KColorBox *m_pShapeBorderColorBox;
	KColorBox *m_pShapeFillColorBox;
};

#endif
