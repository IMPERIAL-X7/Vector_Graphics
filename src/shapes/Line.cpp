#include "shapes/Lines.h"

Line:: Line(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void Line::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);

    p.drawLine(p1, p2);
}

void Line::setBoundingBox(const QPointF& p1, const QPointF& p2)
{
    setP1(p1); setP2(p2);
    // QRectF box_ = QRectF(p1, p2);
    // bBox = box_.normalized();
}

void Line::setP1(const QPointF& p) {p1 = p;}
void Line::setP2(const QPointF& p) {p2 = p;}

std::string Line::toSVG() const
{
    return "";
}

std::shared_ptr<GraphicsObject> Line::clone() const
{
    return nullptr;
}