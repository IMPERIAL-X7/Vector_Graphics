#include "shapes/Circle.h"
#include <QString>
// #include <iostream>


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
    double newX = center.x() - mini / 2;
    double newY = center.y() - mini / 2;
    bBox = QRectF(newX, newY, mini, mini);
    // cx = bBox.x(); cy = bBox.y(); r = mini/2;
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
    /*
        <object="circle"
        style="fill, stroke, width"
        cx="" cy ="" r=""
    */
    double cx, cy, r; r= bBox.width()/2.0;
    cx = bBox.x() + r ; cy = bBox.y() + r; 
    std::ostringstream oss;
    oss << "<object=\"circle\"" << "\n"
        << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString() << "\" "  // 8-digit hex with alpha
        << "stroke-width=\"" << strokeWidth << "\" "
        << "fill=\"" << fill.name(QColor::HexArgb).toStdString() << "\"\n" 
        << "cx=\"" << cx << "\" "
        << "cy=\"" << cy << "\" "
        << "r=\"" << r << "\" " << "/>\n";
    return oss.str();
}

std::shared_ptr<GraphicsObject> Circle::loadShape(const std::string& s1, const std::string& s2)
{
    auto style_ = style(s1);
    auto circle = std::make_shared<Circle>(style_.stroke,style_.fill , style_.strokeWidth);
    double cx, cy, r;
    size_t cx_pos = s2.find("cx=\"") + 4;
    size_t cx_end = s2.find("\"", cx_pos);
    cx = std::stod(s2.substr(cx_pos, cx_end - cx_pos));

    size_t cy_pos = s2.find("cy=\"") + 4;
    size_t cy_end = s2.find("\"", cy_pos);
    cy = std::stod(s2.substr(cy_pos, cy_end - cy_pos));

    size_t r_pos = s2.find("r=\"") + 3;
    size_t r_end = s2.find("\"", r_pos);
    r = std::stod(s2.substr(r_pos, r_end - r_pos));
    circle->setBoundingBox(QPointF(cx - r, cy - r), QPointF(cx + r, cy + r));
    return circle;
}

std::shared_ptr<GraphicsObject> Circle::clone() const
{
    auto clone_ = std::make_shared<Circle>(stroke, fill, strokeWidth);
    clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
    return clone_;
}