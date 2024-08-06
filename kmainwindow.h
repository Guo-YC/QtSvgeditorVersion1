#ifndef __K_MAINWINDOW_H_
#define __K_MAINWINDOW_H_

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>


#include "ui_kmainwindow.h"
class KToolBar;
class KSvgMainWindow;
class KCanvasParamsBar;
class KShapeParamsBar;
class QMenuBar;
class QMenu;
class QAction;
class QSvgRenderer;
class KMainWindow : public QWidget
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = Q_NULLPTR);
    ~KMainWindow();
    void initWindows();
    void initConnection();
	
private slots:
    void validateCanvasParams();
    void validateShapeParams();
    void validateCanvasScale(double scale);
    void newFile();
    void openSVG();
    void save();
    void exportPNG();
    void createActions();
    void createMenus();
private:
    Ui::KMainWindowClass ui;
    QVBoxLayout *m_pVLayout;
    QVBoxLayout *m_pParamsBarLayout;
    QHBoxLayout *m_pHLayout;
    KToolBar *m_pToolBar;
    KSvgMainWindow *m_pSvgMainWin;
    KCanvasParamsBar *m_pCanvasParamsBar;
    KShapeParamsBar *m_pShapeParamsBar;

    QMenuBar *m_menuBar;
    QMenu *m_fileMenu;
    QAction *m_newAct;
    QAction *m_openAct;
    QAction *m_saveAct;
    QAction *m_exportAct;
    QSvgRenderer* m_svgRenderer; // 声明指向 QSvgRenderer 的指针
};

#endif
