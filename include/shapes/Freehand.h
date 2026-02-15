#pragma once
#include "core/GraphicsObject.h"
#include <vector>
#include <QPointF>

class Freehand : public GraphicsObject
{
public:
    Freehand(QColor strokeColor, QColor fillColor, double strokeWidth_);
    
    void addPoint(const QPointF& point);
    void draw(QPainter& p) const override;
    std::string toSVG() const override;
    std::shared_ptr<GraphicsObject> clone() const override;
    void setBoundingBox(const QPointF& p1, const QPointF& p2) override;

private:
    std::vector<QPointF> points;
};