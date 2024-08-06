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
    QVBoxLayout* m_pVLayout; // ��ֱ����
    QButtonGroup* m_pBtnGroup; // ��ť��
    KToolButton* m_pMouseBtn;  // ��갴ť
    KToolButton* m_pPenBtn;    // ���ʰ�ť
    KToolButton* m_pLineBtn;   // ֱ�߰�ť
    KToolButton* m_pRectBtn;   // ���ΰ�ť
    KToolButton* m_pCircleBtn; // Բ�ΰ�ť
    KToolButton* m_pTextBtn;   // �ı���ť
    KToolButton* m_pPentagonBtn; // ����ΰ�ť
    KToolButton* m_pPentagramBtn; // ����ǰ�ť
    KToolButton* m_pHexagonBtn; // �����ΰ�ť

};

#endif
