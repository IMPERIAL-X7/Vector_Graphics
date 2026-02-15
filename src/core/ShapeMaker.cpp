#include "core/ShapeMaker.h"

std::shared_ptr<GraphicsObject> ShapeMaker::create(Tool t, QColor strokeColor, QColor fillColor, double strokeWidth_)
{
    if(t == Tool::Circle) return std::make_shared<Circle>( strokeColor,  fillColor,  strokeWidth_);
    else if(t == Tool::Rect) return std::make_shared<Rect>( strokeColor,  fillColor,  strokeWidth_);
    else if(t == Tool::RoundRect) return std::make_shared<RoundRect>( strokeColor,  fillColor,  strokeWidth_);
    else if(t == Tool::Hexagon) return std::make_shared<Hexagon>( strokeColor,  fillColor,  strokeWidth_);
    else if(t == Tool::Line) return std::make_shared<Line>( strokeColor,  fillColor,  strokeWidth_);
    else if(t == Tool::Text) return std::make_shared<Text>( strokeColor,  fillColor,  strokeWidth_);
    else if (t == Tool::Freehand) return std::make_shared<Freehand>(strokeColor, fillColor, strokeWidth_);
    else return nullptr;
}