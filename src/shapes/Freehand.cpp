#include "shapes/Freehand.h"
#include <QPainterPath>
#include <sstream>

Freehand::Freehand(QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    stroke = strokeColor;
    fill = fillColor;
    strokeWidth = strokeWidth_;
}

void Freehand::addPoint(const QPointF& point)
{
    points.push_back(point);
    
    // Update bounding box
    if (points.size() == 1) {
        bBox = QRectF(point, point);
    } else {
        bBox = bBox.united(QRectF(point, point));
    }
}

void Freehand::draw(QPainter& p) const
{
    if (points.size() < 2) return; // Need at least 2 points to draw
    
    QPen pen(stroke);
    pen.setWidthF(strokeWidth);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    
    // Draw lines connecting all points
    for (size_t i = 1; i < points.size(); ++i) {
        p.drawLine(points[i-1], points[i]);
    }
}

std::string Freehand::toSVG() const
{
    if (points.empty()) return "";
    
    std::stringstream ss;
    ss << "<polyline points=\"";
    
    for (size_t i = 0; i < points.size(); ++i) {
        ss << points[i].x() << "," << points[i].y();
        if (i < points.size() - 1) ss << " ";
    }
    
    ss << "\" stroke=\"" << stroke.name().toStdString() 
       << "\" stroke-width=\"" << strokeWidth 
       << "\" fill=\"none\"/>";
    
    return ss.str();
}

std::shared_ptr<GraphicsObject> Freehand::clone() const
{
    auto clone_ = std::make_shared<Freehand>(stroke, fill, strokeWidth);
    for (const auto& pt : points) {
        clone_->addPoint(pt);
    }
    return clone_;
}

void Freehand::setBoundingBox(const QPointF& p1, const QPointF& p2)
{
    // Freehand doesn't use traditional bounding box assignment
    // Points are added via addPoint()
}