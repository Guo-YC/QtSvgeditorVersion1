#ifndef __K_CIRCLE_H_
#define __K_CIRCLE_H_

#include "kshape.h"

class KCircle : public KShape
{
    Q_OBJECT

public:
    KCircle(QObject* parent = Q_NULLPTR);
    ~KCircle();
    virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
    virtual KShapeType getShapeType();
    virtual KShape* clone() override;
};

#endif //__K_CIRCLE_H_
