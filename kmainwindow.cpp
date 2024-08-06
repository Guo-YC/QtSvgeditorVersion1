#include "kmainwindow.h"
#include <QLineEdit>
#include "kshapeparamsbar.h"
#include "ksvgmainwindow.h"
#include "ktoolbar.h"
#include "kcanvasparamsbar.h"
#include "kcanvas.h"
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QSvgRenderer>
KMainWindow::KMainWindow(QWidget *parent)
    : QWidget(parent)
    , m_menuBar(new QMenuBar(this))
    , m_fileMenu(new QMenu(QString::fromLocal8Bit("�ļ�"), this))
    , m_pVLayout(Q_NULLPTR)
    , m_pHLayout(Q_NULLPTR)
    , m_pSvgMainWin(Q_NULLPTR)
    , m_pToolBar(Q_NULLPTR)
    , m_pParamsBarLayout(Q_NULLPTR)    
    , m_pCanvasParamsBar(Q_NULLPTR)
    , m_pShapeParamsBar(Q_NULLPTR)
    , m_svgRenderer(new QSvgRenderer(this)) // ��ʼ�� QSvgRenderer
{
    ui.setupUi(this);
    
    initWindows();
    initConnection();
    createActions();
    createMenus();
}

KMainWindow::~KMainWindow()
{
    delete m_svgRenderer;
    delete m_exportAct;
    delete m_saveAct;
    delete m_openAct;
    delete m_newAct;
    delete m_fileMenu;
    delete m_menuBar;
    delete m_pShapeParamsBar;
    //delete;
}



void KMainWindow::initWindows()
{
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setSpacing(0);

    m_pParamsBarLayout = new QVBoxLayout(this);
    m_pParamsBarLayout->setSpacing(0);
    m_pParamsBarLayout->setMargin(0);

    m_pToolBar = new KToolBar(this);
    m_pSvgMainWin = new KSvgMainWindow(this);
    m_pCanvasParamsBar = new KCanvasParamsBar(QString::fromLocal8Bit("��������"),this);
    m_pShapeParamsBar = new KShapeParamsBar(QString::fromLocal8Bit("ͼ�β���"), this);
    m_pParamsBarLayout->addWidget(m_pCanvasParamsBar);
    m_pParamsBarLayout->addWidget(m_pShapeParamsBar);

    m_pHLayout->addWidget(m_pToolBar);
    m_pHLayout->addWidget(m_pSvgMainWin);
    m_pHLayout->addLayout(m_pParamsBarLayout);

    m_fileMenu->setStyleSheet(
        "QMenu {"
        "   background-color: #b9b9b9;"
        "   color: #FFFFFF;"
        "}"
        "QMenu::item {"
        "   background-color: #b9b9b9;"  // �˵���δ��ѡ��ʱ�ı�����ɫ
        "   color: #FFFFFF;"             // �˵���δ��ѡ��ʱ��������ɫ
        "}"
        "QMenu::item:selected {"
        "   background-color: #000000;"  // �˵��ѡ��ʱ�ı�����ɫ
        "   color: #FFFFFF;"             // �˵��ѡ��ʱ��������ɫ
        "}"
        "Action{background-color: #b9b9b9;}"
    );
    m_menuBar->setStyleSheet(
        "QMenuBar {"
        "   background-color: #b9b9b9;"
        "   font-size: 14px;"
        "   color: #FFFFFF;"
        "   font-family: Microsoft YaHei;"
        "}"
        "QMenuBar::item {"
        "   background-color: #b9b9b9;"
        "   color: #FFFFFF;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #000000;"  // �˵��ѡ��ʱ�ı�����ɫ
        "   color: #FFFFFF;"            // �˵�����ѡ��ʱ��������ɫ
        "}"
        "QMenuBar::item:pressed {"
        "   background-color: #000000;"  // �˵��ѡ��ʱ�ı�����ɫ
        "   color: #FFFFFF;"  // �˵�������ʱ�ı�����ɫ
        "}"
    );
    m_pVLayout->setMenuBar(m_menuBar);
    m_pVLayout->addLayout(m_pHLayout);
    

    setLayout(m_pVLayout);
 }

void KMainWindow::initConnection()
{
    (void)connect(m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pColorBox, &KColorBox::pickedColor, 
        this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);

    (void)connect(m_pShapeParamsBar->m_pBorderWidthBox->m_pParamEdit, &QLineEdit::editingFinished
        , this, &KMainWindow::validateShapeParams);
    (void)connect(m_pShapeParamsBar->m_pShapeBorderColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateShapeParams);
    (void)connect(m_pShapeParamsBar->m_pShapeFillColorBox, &KColorBox::pickedColor,
        this, &KMainWindow::validateShapeParams);

    (void)connect(m_pSvgMainWin, &KSvgMainWindow::updateScale, this, &KMainWindow::validateCanvasScale);
}


void KMainWindow::createActions()
{
    m_newAct = new QAction(QString::fromLocal8Bit("�½��ļ�"), this);
    m_newAct->setShortcut(QKeySequence::New);
    connect(m_newAct, &QAction::triggered, this, &KMainWindow::newFile);

    m_openAct = new QAction(QString::fromLocal8Bit("��SVG"), this);
    m_openAct->setShortcut(QKeySequence::Open);
    connect(m_openAct, &QAction::triggered, this, &KMainWindow::openSVG);

    m_saveAct = new QAction(QString::fromLocal8Bit("����"), this);
    m_saveAct->setShortcut(QKeySequence::Save);
    connect(m_saveAct, &QAction::triggered, this, &KMainWindow::save);

    m_exportAct = new QAction(QString::fromLocal8Bit("����PNG"), this);
    m_exportAct->setShortcut(QString::fromLocal8Bit("Ctrl+E"));
    connect(m_exportAct, &QAction::triggered, this, &KMainWindow::exportPNG);
}

void KMainWindow::createMenus()
{

    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_exportAct);

    m_menuBar->addMenu(m_fileMenu);
}

void KMainWindow::validateCanvasParams()
{
    qint32 width = m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->text().toInt();
    qint32 height = m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit->text().toInt();
    qint32 scale = m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->text().toInt();

    KGlobalData::getGlobalDataIntance()->setCanvasWidth(width);
    KGlobalData::getGlobalDataIntance()->setCanvasHeight(height);
    KGlobalData::getGlobalDataIntance()->setCanvasScale(scale/100.0);
    
    m_pSvgMainWin->m_pCanvas->resize(width* scale / 100.0, height* scale / 100.0);
    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:#%1").arg(KGlobalData::getGlobalDataIntance()->getCanvasColor()));

}

void KMainWindow::validateShapeParams()
{
    qint32 borderWidth = m_pShapeParamsBar->m_pBorderWidthBox->m_pParamEdit->text().toInt();

    KGlobalData::getGlobalDataIntance()->setBorderWidth(borderWidth);
    
}

void KMainWindow::validateCanvasScale(double scale)
{
    m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->setText(QString::number(qint32(scale * 100)));
}

void KMainWindow::newFile()
{
    m_pSvgMainWin->m_pCanvas->clear();
}

void KMainWindow::openSVG()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("��SVG�ļ�"), "", tr("SVG�ļ� (*.svg)"));
    if (!fileName.isEmpty()) 
    {
        if (m_svgRenderer->load(fileName)) 
        {
            m_pSvgMainWin->m_pCanvas->setRenderer(m_svgRenderer); // ������Ⱦ��
            m_pSvgMainWin->m_pCanvas->update(); // ���»���
        }
        else {
            // �������ʧ�ܵ����
        }
    }
}

void KMainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("����SVG�ļ�"), "", tr("SVG(*.svg)"));
    if (!fileName.isEmpty()) 
    {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(m_pSvgMainWin->m_pCanvas->size());
        generator.setViewBox(m_pSvgMainWin->m_pCanvas->rect());
        generator.setTitle(QString::fromLocal8Bit("SVG����"));
        generator.setDescription(QString::fromLocal8Bit("ʹ��QSvgGenerator������SVG�ļ�"));

        QPainter painter;
        painter.begin(&generator);
        m_pSvgMainWin->m_pCanvas->render(&painter);
        painter.end();
    }
}

void KMainWindow::exportPNG()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("����PNG�ļ�"), "", tr("PNG(*.png)"));
    if (!fileName.isEmpty()) 
    {
        // ��ȡ�������ݲ�����ΪPNG
        QPixmap pixmap(m_pSvgMainWin->m_pCanvas->size());
        m_pSvgMainWin->m_pCanvas->render(&pixmap);

        if (!pixmap.save(fileName, "PNG")) 
        {
            // ������ʧ�ܵ����
        }
    }
}
