#pragma once
#include "core/Tool.h"
#include "shapes/Circle.h"
#include "shapes/Freehand.h"
#include "shapes/Hexagon.h"
#include "shapes/Lines.h"
#include "shapes/Rect.h"
#include "shapes/RoundRect.h"
#include "shapes/Text.h"

// Simple Factory wrapper to instantiate shapes based on the selected tool
class ShapeMaker {
 public:
  static std::shared_ptr<GraphicsObject> create(Tool t, QColor strokeColor,
                                                QColor fillColor,
                                                double strokeWidth_);
};
