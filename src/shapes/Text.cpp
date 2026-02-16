#include "shapes/Text.h"

#include <QFontMetricsF>
#include <iostream>

Text::Text(QColor strokeColor, QColor fillColor, double strokeWidth_) {
  stroke = strokeColor;
  fill = fillColor;
  strokeWidth = strokeWidth_;
}

void Text::draw(QPainter& p) const {
  QPen pen(stroke);
  pen.setWidthF(strokeWidth);
  p.setPen(pen);

  if (fill != Qt::NoBrush)
    p.setBrush(fill);
  else
    p.setBrush(Qt::NoBrush);
  if (text.isEmpty() || bBox.width() <= 0 || bBox.height() <= 0) return;
  p.save();
  // const QTextF b_ = bBox;
  QFont font = p.font();
  font.setPointSizeF(100);
  QFontMetricsF fm(font);
  QRectF boundRect_ = fm.boundingRect(bBox, Qt::AlignCenter, text);
  double scaleX_ = bBox.width() / boundRect_.width();
  double scaleY_ = bBox.height() / boundRect_.height();

  double finalScale_ = std::min(scaleX_, scaleY_) * 0.95;
  font.setPointSizeF(finalScale_ * 100.0);

  p.setFont(font);
  p.drawText(bBox, Qt::AlignCenter, text);
  p.restore();
}

void Text::setText(const QString& str) { text = str; }

std::string Text::toSVG() const {
  double x = bBox.x();
  double y = bBox.y();
  double width = bBox.width();
  double height = bBox.height();
  std::ostringstream oss;
  oss << "<text" << "\n"
      << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString()
      << "\" "  // 8-digit hex with alpha
      << "stroke-width=\"" << strokeWidth << "\" " << "fill=\""
      << fill.name(QColor::HexArgb).toStdString() << "\"\n"
      << "x=\"" << x << "\" " << "y=\"" << y << "\" " << "width=\"" << width
      << "\" " << "height=\"" << height << "\" " << "content=\""
      << text.toStdString() << "\" " << "/>\n";
  return oss.str();
  // multiline check
}

std::shared_ptr<GraphicsObject> Text::clone() const {
  auto clone_ = std::make_shared<Text>(stroke, fill, strokeWidth);
  clone_->setBoundingBox(this->bBox.topLeft(), this->bBox.bottomRight());
  return clone_;
}

std::shared_ptr<GraphicsObject> Text::loadShape(const std::string& s1,
                                                const std::string& s2) {
  auto style_ = style(s1);
  auto text =
      std::make_shared<Text>(style_.stroke, style_.fill, style_.strokeWidth);

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

  size_t content_pos = s2.find("content=\"") + 9;
  size_t content_end = s2.find("\"", content_pos);
  std::string textContent = s2.substr(content_pos, content_end - content_pos);
  std::cout << "Loaded Text: " << textContent << std::endl;

  text->setBoundingBox(QPointF(x, y), QPointF(x + width, y + height));
  std::cout << "Bounding Box: (" << x << ", " << y << ") to (" << x + width
            << ", " << y + height << ")" << std::endl;
  text->setText(QString::fromStdString(textContent));
  return text;
}