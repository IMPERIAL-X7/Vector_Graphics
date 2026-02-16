#pragma once
#include "core/GraphicsObject.h"

class Rect : public GraphicsObject
{
public:
    Rect(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;
    static std::shared_ptr<GraphicsObject> loadShape(const std::string& s1, const std::string& s2);
private:
    double x, y, width, height;
};