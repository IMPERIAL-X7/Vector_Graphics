#pragma once
#include "core/GraphicsObject.h"

class Rect : public GraphicsObject
{
public:
    Rect(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;

private:
    double x, y, width, height;
};