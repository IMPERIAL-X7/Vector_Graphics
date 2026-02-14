#pragma once
#include "core/GraphicsObject.h"

/*
    public allows instances of Circle 
    to be treated as an instance of class GraphicObject too.

*/
class Circle : public GraphicsObject 
{
public:
    Circle(QColor strokeColor, QColor fillColor, double strokeWidth_);

    void resizeBoundingBox();
    // void setBoundingBox(const QPointF& p1, const QPointF& p2) override;
    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;

private:
    double cx, cy, r;
};