#ifndef __K_SVG_TOOL_BAR_H_
#define __K_SVG_TOOL_BAR_H_

#include <QMenuBar>
#include <QtSvg/qsvgrenderer.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qstyleoption.h>
#include <qlayout.h>

#include "ktoolbutton.h"

class KToolBar : public QWidget
{
	Q_OBJECT

public:
	KToolBar(QWidget *parent);
	~KToolBar();
private:
    QVBoxLayout* m_pVLayout; // 垂直布局
    QButtonGroup* m_pBtnGroup; // 按钮组
    KToolButton* m_pMouseBtn;  // 鼠标按钮
    KToolButton* m_pPenBtn;    // 画笔按钮
    KToolButton* m_pLineBtn;   // 直线按钮
    KToolButton* m_pRectBtn;   // 矩形按钮
    KToolButton* m_pCircleBtn; // 圆形按钮
    KToolButton* m_pTextBtn;   // 文本按钮
    KToolButton* m_pPentagonBtn; // 五边形按钮
    KToolButton* m_pPentagramBtn; // 五角星按钮
    KToolButton* m_pHexagonBtn; // 六边形按钮

};

#endif
