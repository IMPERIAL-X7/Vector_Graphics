#include "shapes/Rect.h"

Rect::Rect(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void Rect::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);
    // const QRectF b_ = bBox;
    p.drawRect(bBox);
}

std::string Rect::toSVG() const
{
    return "";
}

std::shared_ptr<GraphicsObject> Rect::clone() const 
{
    return std::make_shared<Rect>(*this);
}
