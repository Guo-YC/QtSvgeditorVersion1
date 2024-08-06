#ifndef __K_LINE_H_
#define __K_LINE_H_

#include "kshape.h"

class KLine : public KShape
{
	Q_OBJECT

public:
	KLine(QObject *parent = Q_NULLPTR);
	~KLine();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual KShape* clone() override;
	virtual KShapeType getShapeType();
};

#endif //__K_LINE_H_
