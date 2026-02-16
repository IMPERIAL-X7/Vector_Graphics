#pragma once
#include <string>

#include "core/Diagram.h"

class XMLParser {
 public:
  static Diagram parse(const std::string& filename);
};