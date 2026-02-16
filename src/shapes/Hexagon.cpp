#include "shapes/Hexagon.h"

Hexagon::Hexagon(QColor strokeColor, QColor fillColor, double strokeWidth_) {
  stroke = strokeColor;
  fill = fillColor;
  strokeWidth = strokeWidth_;
}

void Hexagon::resizeBoundingBox() {
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

void Hexagon::draw(QPainter& p) const {
  QPen pen(stroke);
  pen.setWidthF(strokeWidth);
  p.setPen(pen);

  if (fill != Qt::NoBrush)
    p.setBrush(fill);
  else
    p.setBrush(Qt::NoBrush);
  /*
      QPointF stores numbers as double(or as float in some very old
     arch.[32-bit]), while QPoint stores numbers as int
  */
  QPolygonF polygon;
  double cx = bBox.x() + bBox.width() / 2.0;
  double cy = bBox.y() + bBox.height() / 2.0;
  double a = std::min(bBox.height(), bBox.width()) / 2.0;
  polygon << QPointF(cx + a, cy) << QPointF(cx + a / 2.0, cy + a * 1.732 / 2.0)
          << QPointF(cx - a / 2.0, cy + a * 1.732 / 2.0) << QPointF(cx - a, cy)
          << QPointF(cx - a / 2.0, cy - a * 1.732 / 2.0)
          << QPointF(cx + a / 2.0, cy - a * 1.732 / 2.0);
  p.drawConvexPolygon(polygon);
}

std::string Hexagon::toSVG() const {
  double x, y, width, height;
  x = bBox.x();
  y = bBox.y();
  width = bBox.width();
  std::ostringstream oss;
  oss << "<hexagon" << "\n"
      << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString()
      << "\" "  // 8-digit hex with alpha
      << "stroke-width=\"" << strokeWidth << "\" " << "fill=\""
      << fill.name(QColor::HexArgb).toStdString() << "\"\n"
      << "x=\"" << x << "\" " << "y=\"" << y << "\" " << "width=\"" << width
      << "\" " << "/>\n";
  return oss.str();
}

std::shared_ptr<GraphicsObject> Hexagon::clone() const {
  auto clone_ = std::make_shared<Hexagon>(stroke, fill, strokeWidth);
  clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
  return clone_;
}

void Hexagon::setWidth(float w) { width = w; }

std::shared_ptr<GraphicsObject> Hexagon::loadShape(const std::string& s1,
                                                   const std::string& s2) {
  auto style_ = style(s1);
  auto hexagon =
      std::make_shared<Hexagon>(style_.stroke, style_.fill, style_.strokeWidth);

  double x, y, width;
  size_t x_pos = s2.find("x=\"") + 3;
  size_t x_end = s2.find("\"", x_pos);
  x = std::stod(s2.substr(x_pos, x_end - x_pos));

  size_t y_pos = s2.find("y=\"") + 3;
  size_t y_end = s2.find("\"", y_pos);
  y = std::stod(s2.substr(y_pos, y_end - y_pos));

  size_t w_pos = s2.find("width=\"") + 7;
  size_t w_end = s2.find("\"", w_pos);
  width = std::stod(s2.substr(w_pos, w_end - w_pos));

  hexagon->setBoundingBox(QPointF(x, y), QPointF(x + width, y + width));
  return hexagon;
}
