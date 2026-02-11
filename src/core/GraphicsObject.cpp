#include "core/GraphicsObject.h"

GraphicsObject::~GraphicsObject() = default;

void GraphicsObject::setStroke(const QColor& s)
{
    stroke = s;
}

void GraphicsObject::setFill(const QColor& f)
{
    fill = f;
}

void GraphicsObject::setStrokeWidth(float w)
{
    strokeWidth = w;
}

const QRectF& GraphicsObject::boundingBox() const
{
    return bBox;
}

void GraphicsObject::setBoundingBox(const QPointF& p1, const QPointF& p2)
{
    QRectF box_ = QRectF(p1, p2);
    bBox = box_.normalized();
}
