#include "ktext.h"
#include "kglobaldata.h"
#include <QPainter>


KText::KText(QObject* parent)
    : KShape(parent), m_text(QString::fromLocal8Bit("�ı���"))
{
}

KText::~KText()
{
}

void KText::drawShape(QPaintDevice* parent)
{
    QPainter painter(parent);    
    // ���ñ߿�����ɫ�Ϳ��
    QPen pen;
    pen.setStyle(m_penStyle);
    pen.setColor(QColor("#" + m_borderColor)); // ȷ����ɫ��������Ч��
    pen.setWidth(m_borderWidth);
    painter.setPen(pen);
    painter.scale(KGlobalData::getGlobalDataIntance()->getCanvasScale(), KGlobalData::getGlobalDataIntance()->getCanvasScale());

    // ���������ɫ
    QBrush brush(QColor("#" + m_fillColor));
    painter.setBrush(brush);
    // ��������������
    QFont font("Arial", 16);  // ѡ�� Arial ���壬��СΪ 16
    painter.setFont(font);
    // ��������ı��ľ�������
    QPoint startPoint = getStartPoint();
    QPoint endPoint = getEndPoint();
    QRect textRect(startPoint, endPoint);

    // �����ı���ʹ�� Qt::AlignCenter ���뷽ʽ�������ı�
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
