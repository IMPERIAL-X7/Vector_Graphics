#include "shapes/RoundRect.h"

#include <iostream>
RoundRect::RoundRect(QColor strokeColor, QColor fillColor,
                     double strokeWidth_) {
  stroke = strokeColor;
  fill = fillColor;
  strokeWidth = strokeWidth_;
}

void RoundRect::draw(QPainter& p) const {
  QPen pen(stroke);
  pen.setWidthF(strokeWidth);
  p.setPen(pen);

  if (fill != Qt::NoBrush)
    p.setBrush(fill);
  else
    p.setBrush(Qt::NoBrush);
  p.drawRoundedRect(bBox, radius, radius);
}

void RoundRect::setRadius(double r) { radius = r; }

double RoundRect::viewRadius() const { return radius; }

std::string RoundRect::toSVG() const {
  double x, y, width, height, radius_;
  x = bBox.x();
  y = bBox.y();
  width = bBox.width();
  height = bBox.height();
  radius_ = radius;
  std::ostringstream oss;
  oss << "<roundrect" << "\n"
      << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString()
      << "\" "  // 8-digit hex with alpha
      << "stroke-width=\"" << strokeWidth << "\" " << "fill=\""
      << fill.name(QColor::HexArgb).toStdString() << "\"\n"
      << "x=\"" << x << "\" " << "y=\"" << y << "\" " << "width=\"" << width
      << "\" " << "height=\"" << height << "\" " << "rx=\"" << radius_ << "\" "
      << "ry=\"" << radius_ << "\" " << "/>\n";
  return oss.str();
}

std::shared_ptr<GraphicsObject> RoundRect::loadShape(const std::string& s1,
                                                     const std::string& s2) {
  auto style_ = style(s1);
  auto rect = std::make_shared<RoundRect>(style_.stroke, style_.fill,
                                          style_.strokeWidth);

  double x, y, width, height, radius_;
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

  size_t r_pos = s2.find("rx=\"") + 4;
  size_t r_end = s2.find("\"", r_pos);
  radius_ = std::stod(s2.substr(r_pos, r_end - r_pos));

  rect->setBoundingBox(QPointF(x, y), QPointF(x + width, y + height));
  rect->setRadius(radius_);
  std::cout << "Loaded RoundRect with x: " << x << ", y: " << y
            << ", width: " << width << ", height: " << height
            << ", radius: " << radius_ << std::endl;
  return rect;
}

std::shared_ptr<GraphicsObject> RoundRect::clone() const {
  auto clone_ = std::make_shared<RoundRect>(stroke, fill, strokeWidth);
  clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
  clone_->setRadius(this->radius);
  return clone_;
}