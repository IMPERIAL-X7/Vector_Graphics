#include "io/XMLParser.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "shapes/Circle.h"
#include "shapes/Freehand.h"
#include "shapes/Hexagon.h"
#include "shapes/Lines.h"
#include "shapes/Rect.h"
#include "shapes/RoundRect.h"
#include "shapes/Text.h"

// Parses the custom SVG format saved by SVGWriter
// IMPORTANT: This parser assumes each shape is serialized into exactly 3 lines:
// Line 1: The opening tag (e.g., <circle)
// Line 2: Style attributes (stroke, fill, width)
// Line 3: Geometry attributes (coords, dimensions) and closing tag
CanvasState XMLParser::parse(const std::string& filename) {
  CanvasState state(nullptr, 0,
                    0);  // Default state with null diagram and zero dimensions
  auto d =
      std::make_shared<Diagram>();  // Create a new Diagram instance to populate
  std::ifstream file(filename);
  if (!file.is_open()) return state;
  std::string line;

  // Skip header lines until <svg tag is found
  if (std::getline(file, line) && line.find("<svg") != std::string::npos) {
    size_t w = line.find("width=\"");
    size_t h = line.find("height=\"");
    if (w != std::string::npos && h != std::string::npos) {
      // Extract width and height values (currently unused)
      size_t w_start = w + 7;
      size_t w_end = line.find("\"", w_start);
      size_t h_start = h + 8;
      size_t h_end = line.find("\"", h_start);
      int width = std::stoi(line.substr(w_start, w_end - w_start));
      int height = std::stoi(line.substr(h_start, h_end - h_start));
      state.height = height;
      state.width = width;
      std::cout << "Parsed SVG dimensions: width=" << width
                << ", height=" << height << std::endl;
    }
  }

  while (std::getline(file, line)) {
    if (line.find("</svg>") != std::string::npos) break;
    if (line.empty()) continue;

    // Read the next two lines expecting specific attributes
    std::string s1, s2;
    getline(file, s1);  // Style line
    getline(file, s2);  // Geometry line

    if (line.find("circle") != std::string::npos) {
      std::shared_ptr<GraphicsObject> circle_ = Circle::loadShape(s1, s2);
      d->add(circle_);
    } else if (line.find("roundrect") != std::string::npos) {
      std::shared_ptr<GraphicsObject> roundRect_ = RoundRect::loadShape(s1, s2);
      d->add(roundRect_);
    } else if (line.find("rect") != std::string::npos) {
      std::shared_ptr<GraphicsObject> rect_ = Rect::loadShape(s1, s2);
      d->add(rect_);
    } else if (line.find("hexagon") != std::string::npos) {
      std::shared_ptr<GraphicsObject> hexagon_ = Hexagon::loadShape(s1, s2);
      d->add(hexagon_);
    } else if (line.find("text") != std::string::npos) {
      std::shared_ptr<GraphicsObject> text_ = Text::loadShape(s1, s2);
      d->add(text_);
    } else if (line.find("line") != std::string::npos) {
      std::shared_ptr<GraphicsObject> line_ = Line::loadShape(s1, s2);
      d->add(line_);
    } else if (line.find("freehand") != std::string::npos) {
      std::shared_ptr<GraphicsObject> freehand_ = Freehand::loadShape(s1, s2);
      d->add(freehand_);
    }
  }
  state.diagram = d;
  return state;
}