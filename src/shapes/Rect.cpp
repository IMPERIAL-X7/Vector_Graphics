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
    double x, y, width, height;
    x= bBox.x();
    y= bBox.y();
    width = bBox.width();
    height = bBox.height();
    std::ostringstream oss;
    oss << "<object=\"rect\"" << "\n"
        << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString() << "\" "  // 8-digit hex with alpha
        << "stroke-width=\"" << strokeWidth << "\" "
        << "fill=\"" << fill.name(QColor::HexArgb).toStdString() << "\"\n" 
        << "x=\"" << x << "\" "
        << "y=\"" << y << "\" "
        << "width=\"" << width << "\" "
        << "height=\"" << height << "\" " << "/>\n";
    return oss.str();
}

std::shared_ptr<GraphicsObject> Rect::clone() const 
{
    auto clone_ = std::make_shared<Rect>(stroke, fill, strokeWidth);
    clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
    return clone_;
}

std::shared_ptr<GraphicsObject> Rect::loadShape(const std::string& s1, const std::string& s2)
{
    auto style_ = style(s1);
    auto rect = std::make_shared<Rect>(style_.stroke,style_.fill , style_.strokeWidth);
    
    double x, y, width, height;
    size_t x_pos = s2.find("x=\"") + 3;
    size_t x_end = s2.find("\"", x_pos);
    x = std::stod(s2.substr(x_pos, x_end - x_pos));

    size_t y_pos = s2.find("y=\"") + 3;
    size_t y_end = s2.find("\"", y_pos);
    y = std::stod(s2.substr(y_pos, y_end - y_pos));

    size_t w_pos = s2.find("width=\"") + 7;
    size_t w_end = s2.find("\"", w_pos);
    width = std::stod(s2.substr(w_pos, w_end - w_pos));

    size_t h_pos = s2.find("height=\"") + 8;
    size_t h_end = s2.find("\"", h_pos);
    height = std::stod(s2.substr(h_pos, h_end - h_pos));

    rect->setBoundingBox(QPointF(x, y), QPointF(x+width, y+height));
    return rect;
}