#include "core/Diagram.h"

void Diagram:: add(const std::shared_ptr<GraphicsObject>& obj)
{
    objs.push_back(obj);
}

const std::vector<std::shared_ptr<GraphicsObject>>& Diagram:: objects() const
{
    return objs;
}

void Diagram::clear()
{
    objs.clear();
}