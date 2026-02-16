#pragma once
#include <QPainterPath>
#include <QPointF>
#include <iostream>
#include <sstream>
#include <vector>

#include "core/GraphicsObject.h"

class Freehand : public GraphicsObject {
 public:
  Freehand(QColor strokeColor, QColor fillColor, double strokeWidth_);

  void addPoint(const QPointF& point);
  void draw(QPainter& p) const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setBoundingBox(const QPointF& p1, const QPointF& p2) override;
  static std::shared_ptr<GraphicsObject> loadShape(const std::string& s1,
                                                   const std::string& s2);

 private:
  std::vector<QPointF> points;
};