#pragma once
#include <string>

#include "core/Diagram.h"

class XMLParser {
 public:
  static CanvasState parse(const std::string& filename);
};