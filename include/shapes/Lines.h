#pragma once
#include "core/GraphicsObject.h"

class Line : public GraphicsObject {
 public:
  Line(QColor strokeColor, QColor fillColor, double strokeWidth_);

  void draw(QPainter& p) const override;
  std::string toSVG() const override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setBoundingBox(const QPointF& p1, const QPointF& p2) override;
  static std::shared_ptr<GraphicsObject> loadShape(const std::string& s1,
                                                   const std::string& s2);
  void setP1(const QPointF& p);
  void setP2(const QPointF& p);

 private:
  QPointF p1;
  QPointF p2;
};