#ifndef __K_TEXT_H_
#define __K_TEXT_H_

#include "kshape.h"
#include <QString>

class KText : public KShape
{
    Q_OBJECT

public:
    KText(QObject* parent = Q_NULLPTR);
    ~KText();
    virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
    virtual KShape* clone() override;
    virtual KShapeType getShapeType();
    void setText(const QString& text);
    QString getText() const;

private:
    QString m_text;
};

#endif //__K_TEXT_H_
