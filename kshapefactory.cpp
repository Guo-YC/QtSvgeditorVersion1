#include "kshapefactory.h"
#include "kline.h"
#include "kpen.h"
#include "krect.h"
#include "kcircle.h"
#include "ktext.h"
#include "kpentagon.h"
#include "kpentagram.h"
#include "khexagon.h"

KShapeFactory::KShapeFactory(QObject *parent)
	: QObject(parent)
{
}

KShapeFactory::~KShapeFactory()
{
}

KShape* KShapeFactory::createShape(KGlobalData::KDrawFlag drawFlag)
{
    switch (drawFlag)
    {
    case KGlobalData::KDrawFlag::RectDrawFlag:
        return new KRect;
    case KGlobalData::KDrawFlag::LineDrawFlag:
        return new KLine;
    case KGlobalData::KDrawFlag::PenDrawFlag:
        return new KPen;
    case KGlobalData::KDrawFlag::CircleDrawFlag:
        return new KCircle;
    case KGlobalData::KDrawFlag::TextDrawFlag:
        return new KText;
    case KGlobalData::KDrawFlag::PentagonDrawFlag:
        return new KPentagon;
    case KGlobalData::KDrawFlag::PentagramDrawFlag:
        return new KPentagram;
    case KGlobalData::KDrawFlag::HexagonDrawFlag:
        return new KHexagon;
    default:
        return nullptr;  // Return nullptr if the drawFlag doesn't match any known type
    }
}
