#pragma once
#include "core/GraphicsObject.h"

class Hexagon : public GraphicsObject
{
public:
    Hexagon(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;

    void setWidth(float w);
private:
    double cx, cy, width;
};