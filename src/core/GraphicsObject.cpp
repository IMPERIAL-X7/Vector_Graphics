#include "core/GraphicsObject.h"
// #include<iostream>

GraphicsObject::~GraphicsObject() = default;

void GraphicsObject::setStroke(const QColor& s) { stroke = s; }

void GraphicsObject::setFill(const QColor& f) { fill = f; }

void GraphicsObject::setStrokeWidth(float w) { strokeWidth = w; }

const QRectF& GraphicsObject::boundingBox() const { return bBox; }

void GraphicsObject::setBoundingBox(const QPointF& p1, const QPointF& p2) {
  QRectF box_ = QRectF(p1, p2);
  bBox = box_.normalized();
}

void GraphicsObject::resize(const double factor) {
  QPointF center = bBox.center();
  double w = bBox.width();
  double h = bBox.height();
  double newW = w * factor;
  double newH = h * factor;
  double newX = center.x() - newW / 2.0;
  double newY = center.y() - newH / 2.0;
  bBox = QRectF(newX, newY, newW, newH).normalized();
  // std::cout << toSVG() << std::endl;
}

void GraphicsObject::move(const QLineF l) {
  bBox.translate(l.dx(), l.dy());
  // std::cout << toSVG() << std::endl;
}
