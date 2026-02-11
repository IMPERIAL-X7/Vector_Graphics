#include "shapes/Hexagon.h"

Hexagon::Hexagon(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void Hexagon::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);
    /*
        QPointF stores numbers as double(or as float in some very old arch.[32-bit]), while QPoint stores numbers as int
    */
    QPolygonF polygon; double cx = bBox.x() + bBox.width()/2.0; double cy = bBox.y() + bBox.height()/2.0;
    double a = std::min(bBox.height(), bBox.width())/2.0;
    polygon << QPointF(cx + a, cy) << QPointF(cx + a/2.0, cy + a*1.732/2.0)
            << QPointF(cx - a/2.0, cy + a*1.732/2.0) << QPointF(cx - a, cy)
            << QPointF(cx - a/2.0, cy - a*1.732/2.0) << QPointF(cx + a/2.0, cy - a*1.732/2.0);
    p.drawConvexPolygon(polygon);
}

std::string Hexagon::toSVG() const
{
    return "";
}

std::shared_ptr<GraphicsObject> Hexagon::clone() const 
{
    return nullptr;
}

void Hexagon::setWidth(float w)
{
    width = w;
}