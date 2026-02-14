#include "shapes/Circle.h"
#include <sstream>
#include <QString>


Circle::Circle(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

// void Circle::setBoundingBox(const QPointF& p1, const QPointF& p2)
// {
//     QRectF box_ = QRectF(p1, p2);
//     box_ = box_.normalized();
// }

void Circle::resizeBoundingBox()
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

void Circle::draw(QPainter& p) const
{
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    p.setPen(pen);

    if(fill != Qt::NoBrush) p.setBrush(fill);
    else p.setBrush(Qt::NoBrush);
    // p.drawConvexPolygon()
    float r = std::min(bBox.width() , bBox.height()) / 2.0f;
    QPointF center = bBox.center();
    p.drawEllipse(center, r, r);
}

std::string Circle::toSVG() const
{
    std::ostringstream oss;
    // oss << "<circle " 
    //     << "cx=\"" << cx << "\" "
    //     << "cy=\"" << cy << "\" "
    //     << "r=\"" << r << "\" "
    //     << "stroke=\"" << stroke << "\" "
    //     << "stroke-width=\"" << strokeWidth << "\" "
    //     << "fill=\"" << fill << "\" />\n";
    return oss.str();
}

std::shared_ptr<GraphicsObject> Circle::clone() const
{
    auto clone_ = std::make_shared<Circle>(stroke, fill, strokeWidth);
    clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
    return clone_;
}