#pragma once
#include "core/GraphicsObject.h"

class RoundRect : public GraphicsObject
{
public:
    RoundRect(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;
    void setRadius(double r);
    double viewRadius() const;
private:
    double x, y, width, height;
    double radius;
};