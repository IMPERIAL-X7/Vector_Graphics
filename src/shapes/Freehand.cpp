#include "shapes/Freehand.h"
#include <QPainterPath>
#include <sstream>
#include <iostream>

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
    std::ostringstream oss;
    oss << "<object=\"freehand\"" << "\n"
        << "stroke=\"" << stroke.name(QColor::HexArgb).toStdString() << "\" "
        << "stroke-width=\"" << strokeWidth << "\" "
        << "fill=\"none\"\n"
        << "points=\"";
    for (size_t i = 0; i < points.size(); ++i) {
        if (i > 0) oss << " ";
        oss << points[i].x() << "," << points[i].y();
    }
    oss << "\" />\n";
    return oss.str();
}

std::shared_ptr<GraphicsObject> Freehand::loadShape(const std::string& s1, const std::string& s2)
{
    auto style_ = style(s1);
    auto freehand = std::make_shared<Freehand>(style_.stroke, style_.fill, style_.strokeWidth);

    // Parse points from s2 (format: "x1,y1 x2,y2 x3,y3 ...")
    std::istringstream iss(s2);
    std::string pointPair; iss >> pointPair;
    std::cout << pointPair << std::endl;
    while (iss >> pointPair) {
        size_t commaPos = pointPair.find(','); std::cout << pointPair << std::endl;
        if (commaPos != std::string::npos) {
            double x = std::stod(pointPair.substr(0, commaPos));
            double y = std::stod(pointPair.substr(commaPos + 1));
            freehand->addPoint(QPointF(x, y));
        }
    }
    
    return freehand;
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