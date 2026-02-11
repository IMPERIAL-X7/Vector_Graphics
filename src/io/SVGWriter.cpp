#include "io/SVGWriter.h"
#include <fstream>

void SVGWriter::write(const Diagram& diagram, const std::string& filename)
{
    std::ofstream out(filename);
    for(const auto& obj : diagram.objects())
    {
        out << obj->toSVG();
    }
}
