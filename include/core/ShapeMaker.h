#pragma once
#include "shapes/Circle.h"
#include "shapes/Rect.h"
#include "shapes/RoundRect.h"
#include "shapes/Hexagon.h"
#include "shapes/Lines.h"
#include "shapes/Text.h"
#include "core/Tool.h"
#include "shapes/Freehand.h"

class ShapeMaker
{
public:
    static std::shared_ptr<GraphicsObject> create(Tool t, QColor strokeColor, QColor fillColor, double strokeWidth_);
};
