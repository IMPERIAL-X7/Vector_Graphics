#include "io/SVGWriter.h"

#include <fstream>

#include "ui/Canvas.h"

void SVGWriter::write(const Diagram& diagram, const std::string& filename) {
  std::ofstream out(filename);
  out << "<svg width=\"600\" height=\"400\" >\n";
  for (const auto& obj : diagram.objects()) {
    out << obj->toSVG();
  }
  out << "</svg>";
  out.close();
}
