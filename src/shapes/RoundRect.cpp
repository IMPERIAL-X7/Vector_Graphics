#include "shapes/RoundRect.h"

RoundRect::RoundRect(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void RoundRect::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);
    // const QRectF b_ = bBox;
    p.drawRoundedRect(bBox, radius, radius);
}

void RoundRect::setRadius(double r)
{
    radius = r;
}

double RoundRect::viewRadius() const
{
    return radius;
}

std::string RoundRect::toSVG() const
{
    return "";
}

std::shared_ptr<GraphicsObject> RoundRect::clone() const 
{
    return std::make_shared<RoundRect>(*this);
}
