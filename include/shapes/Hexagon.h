#pragma once
#include "core/GraphicsObject.h"

class Hexagon : public GraphicsObject {
 public:
  Hexagon(QColor strokeColor, QColor fillColor, double strokeWidth_);
  void resizeBoundingBox();
  void draw(QPainter& p) const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
  static std::shared_ptr<GraphicsObject> loadShape(const std::string& s1,
                                                   const std::string& s2);

  void setWidth(float w);

 private:
  double cx, cy, width;
};