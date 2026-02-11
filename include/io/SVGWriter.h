#pragma once
#include "core/Diagram.h"

class SVGWriter
{
public:
    static void write(const Diagram& diagram, const std::string& filename);
};

