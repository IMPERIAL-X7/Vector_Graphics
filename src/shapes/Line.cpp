#include "shapes/Lines.h"

Line::Line(QColor strokeColor, QColor fillColor, double strokeWidth_) {
  stroke = strokeColor;
  fill = fillColor;
  strokeWidth = strokeWidth_;
}

void Line::draw(QPainter& p) const {
  QPen pen(stroke);
  pen.setWidthF(strokeWidth);
  p.setPen(pen);

  if (fill != Qt::NoBrush)
    p.setBrush(fill);
  else
    p.setBrush(Qt::NoBrush);

  p.drawLine(p1, p2);
}

void Line::setBoundingBox(const QPointF& p1, const QPointF& p2) {
  setP1(p1);
  setP2(p2);
}

void Line::setP1(const QPointF& p) { p1 = p; }
void Line::setP2(const QPointF& p) { p2 = p; }

std::string Line::toSVG() const {
  std::ostringstream oss;
  oss << "<line" << "\n"
      << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString()
      << "\" "  // 8-digit hex with alpha
      << "stroke-width=\"" << strokeWidth << "\" " << "fill=\""
      << fill.name(QColor::HexArgb).toStdString() << "\"\n"
      << "x1=\"" << p1.x() << "\" " << "y1=\"" << p1.y() << "\" " << "x2=\""
      << p2.x() << "\" " << "y2=\"" << p2.y() << "\" " << "/>\n";
  return oss.str();
}

std::shared_ptr<GraphicsObject> Line::loadShape(const std::string& s1,
                                                const std::string& s2) {
  auto style_ = style(s1);
  auto line =
      std::make_shared<Line>(style_.stroke, style_.fill, style_.strokeWidth);

  double x1, y1, x2, y2;
  size_t x1_pos = s2.find("x1=\"") + 4;
  size_t y1_pos = s2.find("y1=\"") + 4;
  size_t x2_pos = s2.find("x2=\"") + 4;
  size_t y2_pos = s2.find("y2=\"") + 4;

  if (x1_pos == std::string::npos || y1_pos == std::string::npos ||
      x2_pos == std::string::npos || y2_pos == std::string::npos) {
    return nullptr;
  }

  x1 = std::stod(s2.substr(x1_pos, s2.find("\"", x1_pos) - x1_pos));
  y1 = std::stod(s2.substr(y1_pos, s2.find("\"", y1_pos) - y1_pos));
  x2 = std::stod(s2.substr(x2_pos, s2.find("\"", x2_pos) - x2_pos));
  y2 = std::stod(s2.substr(y2_pos, s2.find("\"", y2_pos) - y2_pos));

  line->setP1(QPointF(x1, y1));
  line->setP2(QPointF(x2, y2));

  return line;
}

std::shared_ptr<GraphicsObject> Line::clone() const {
  auto line_ = std::make_shared<Line>(stroke, fill, strokeWidth);
  line_->setP1(p1 + QPointF(10, 10));  // Offset the cloned line for visibility
  line_->setP2(p2 + QPointF(10, 10));
  return line_;
}