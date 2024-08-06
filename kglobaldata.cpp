#include "kglobaldata.h"
#include <Windows.h>
#include <QPen>


KGlobalData::KGlobalData(QObject* parent)
    : QObject(parent)
    , m_drawFlag(KDrawFlag::NoneDrawFlag)
    , m_canvasColor("FFFFFF")
    , m_shapeBorderColor("000000")
    , m_shapeFillColor("FFFFFF")
    , m_canvasWidth(600)
    , m_canvasHeight(400)
    , m_scale(1.0)
    , m_borderWidth(2)
    , m_borderStyle(Qt::PenStyle::SolidLine)
{
}



KGlobalData *KGlobalData::getGlobalDataIntance()
{
    static KGlobalData s_instance;
    static KGlobalData* pinstance = nullptr;
    if (pinstance == nullptr)
        pinstance = &s_instance;
    return pinstance;
}

void KGlobalData::setDrawFlag(KGlobalData::KDrawFlag drawflag)
{
    this->m_drawFlag = drawflag;
}

KGlobalData::KDrawFlag KGlobalData::getDrawFlag()
{
    return m_drawFlag;
}

void KGlobalData::setCanvasWidth(const int width)
{
    this->m_canvasWidth = width;
}

void KGlobalData::setCanvasHeight(const int height)
{
    this->m_canvasHeight = height;
}

void KGlobalData::setBorderWidth(const int width)
{
    this->m_borderWidth = width;
}


int KGlobalData::getCanvasWidth() const
{
    return this->m_canvasWidth;
}

int KGlobalData::getBorderWidth() const
{
    return this->m_borderWidth;
}

int KGlobalData::getCanvasHeight() const
{
    return m_canvasHeight;
}

void KGlobalData::setCanvasColor(const QString& colorStr)
{
    this->m_canvasColor = colorStr;
}

void KGlobalData::setShapeBorderColor(const QString& colorStr)
{
    this->m_shapeBorderColor = colorStr;
}

QString KGlobalData::getShapeBorderColor()
{
    return m_shapeBorderColor;
}

void KGlobalData::setShapeFillColor(const QString& colorStr)
{
    this->m_shapeFillColor = colorStr;
}

QString KGlobalData::getShapeFillColor()
{
    return m_shapeFillColor;
}

QString KGlobalData::getCanvasColor()
{
    return m_canvasColor;
}

void KGlobalData::setCanvasScale(qreal scale)
{
    m_scale = scale;    
}

qreal KGlobalData::getCanvasScale()
{
    return m_scale;
}

void KGlobalData::setBorderStyle(Qt::PenStyle style)
{
    m_borderStyle = style;
}

Qt::PenStyle KGlobalData::getBorderStyle()
{
    return m_borderStyle;
}



KGlobalData::~KGlobalData()
{
}
