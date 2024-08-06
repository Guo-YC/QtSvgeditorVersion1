#include "ktext.h"
#include "kglobaldata.h"
#include <QPainter>


KText::KText(QObject* parent)
    : KShape(parent), m_text(QString::fromLocal8Bit("文本框"))
{
}

KText::~KText()
{
}

void KText::drawShape(QPaintDevice* parent)
{
    QPainter painter(parent);    
    // 设置边框风格、颜色和宽度
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // 确保颜色代码是有效的
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());

    // 设置填充颜色
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
    // 创建并设置字体
    QFont font("Arial", 16);  // 选择 Arial 字体，大小为 16
    painter.setFont(font);
    // 计算绘制文本的矩形区域
    QPoint startPoint = getStartPoint();
    QPoint endPoint = getEndPoint();
    QRect textRect(startPoint, endPoint);

    // 绘制文本，使用 Qt::AlignCenter 对齐方式来居中文本
    painter.drawText(textRect, m_text);
}

KShape* KText::clone()
{
    KText* p = new KText(*this);
    p->setText(getText());
    return p;
}

KShapeType KText::getShapeType()
{
    return KShapeType::TextShapeType;
}

void KText::setText(const QString& text)
{
    m_text = text;
}

QString KText::getText() const
{
    return m_text;
}
