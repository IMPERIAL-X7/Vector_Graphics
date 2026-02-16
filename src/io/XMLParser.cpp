#include "io/XMLParser.h"

#include <fstream>
#include <sstream>

#include "shapes/Circle.h"
#include "shapes/Freehand.h"
#include "shapes/Hexagon.h"
#include "shapes/Lines.h"
#include "shapes/Rect.h"
#include "shapes/RoundRect.h"
#include "shapes/Text.h"

Diagram XMLParser::parse(const std::string& filename) {
  Diagram d;
  std::ifstream file(filename);
  if (!file.is_open()) return d;
  std::string line;

  if (std::getline(file, line) && line.find("<svg") != std::string::npos) {
    size_t w = line.find("width=\"");
    size_t h = line.find("height=\"");
  }

  while (std::getline(file, line)) {
    if (line.find("</svg>") != std::string::npos) break;
    if (line.empty()) continue;
    std::string s1, s2;
    getline(file, s1);
    getline(file, s2);
    if (line.find("circle") != std::string::npos) {
      std::shared_ptr<GraphicsObject> circle_ = Circle::loadShape(s1, s2);
      d.add(circle_);
    } else if (line.find("roundrect") != std::string::npos) {
      std::shared_ptr<GraphicsObject> roundRect_ = RoundRect::loadShape(s1, s2);
      d.add(roundRect_);
    } else if (line.find("rect") != std::string::npos) {
      std::shared_ptr<GraphicsObject> rect_ = Rect::loadShape(s1, s2);
      d.add(rect_);
    } else if (line.find("hexagon") != std::string::npos) {
      std::shared_ptr<GraphicsObject> hexagon_ = Hexagon::loadShape(s1, s2);
      d.add(hexagon_);
    } else if (line.find("text") != std::string::npos) {
      std::shared_ptr<GraphicsObject> text_ = Text::loadShape(s1, s2);
      d.add(text_);
    } else if (line.find("line") != std::string::npos) {
      std::shared_ptr<GraphicsObject> line_ = Line::loadShape(s1, s2);
      d.add(line_);
    } else if (line.find("freehand") != std::string::npos) {
      std::shared_ptr<GraphicsObject> freehand_ = Freehand::loadShape(s1, s2);
      d.add(freehand_);
    }
  }
  return d;
}