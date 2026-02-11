#include "shapes/Circle.h"
#include <sstream>
#include <QString>


Circle::Circle(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
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
    return std::make_shared<Circle>(*this);
}