#include "io/XMLParser.h"
#include "shapes/Circle.h"
#include <fstream>
#include <sstream>

static bool extractAttribute(
    const std::string& line,
    const std::string& key,
    std::string& value
) {
    std::string pattern = key + "=\"";
    auto pos = line.find(pattern);
    if (pos == std::string::npos) return false;

    pos += pattern.length();
    auto end = line.find("\"", pos);
    if (end == std::string::npos) return false;

    value = line.substr(pos, end - pos);
    return true;
}


Diagram XMLParser::parse(const std::string& filename)
{
    Diagram d;
    // std::ifstream in(filename);
    // std::string line;
    // while (std::getline(in, line)) {

    //     // Only handle circles for now
    //     if (line.find("<circle") != std::string::npos) {

    //         std::string cxStr, cyStr, rStr;
    //         extractAttribute(line, "cx", cxStr);
    //         extractAttribute(line, "cy", cyStr);
    //         extractAttribute(line, "r",  rStr);

    //         float cx = std::stof(cxStr);
    //         float cy = std::stof(cyStr);
    //         float r  = std::stof(rStr);

    //         auto circle = std::make_shared<Circle>();

    //         // // Optional style attributes
    //         std::string stroke, fill, sw;
    //         if (extractAttribute(line, "stroke", stroke))
    //             circle->setStroke(stroke);

    //         if (extractAttribute(line, "fill", fill))
    //             circle->setFill(fill);

    //         if (extractAttribute(line, "stroke-width", sw))
    //             circle->setStrokeWidth(std::stof(sw));

    //         d.add(circle);
    //     }
    // }

    return d;
}