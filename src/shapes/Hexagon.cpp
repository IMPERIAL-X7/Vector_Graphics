#include "shapes/Hexagon.h"

Hexagon::Hexagon(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

// void Hexagon::setBoundingBox(const QPointF& p1, const QPointF& p2)
// {
//     QRectF box_ = QRectF(p1, p2);
//     box_ = box_.normalized();
// }

void Hexagon::resizeBoundingBox()
{
    double mini = std::min(bBox.width(), bBox.height());
    QPointF center = bBox.center();
    double w = bBox.width();
    double h = bBox.height();
    double newW = mini;
    double newH = mini;
    double newX = center.x() - newW / 2;
    double newY = center.y() - newH / 2;
    bBox = QRectF(newX, newY, newW, newH);
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
    auto clone_ = std::make_shared<Hexagon>(stroke, fill, strokeWidth);
    clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
    return clone_;
}

void Hexagon::setWidth(float w)
{
    width = w;
}