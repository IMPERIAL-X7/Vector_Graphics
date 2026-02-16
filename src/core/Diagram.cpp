#include "core/Diagram.h"

void Diagram::add(std::shared_ptr<GraphicsObject>& obj) { objs.push_back(obj); }

const std::vector<std::shared_ptr<GraphicsObject>>& Diagram::objects() const {
  return objs;
}

void Diagram::destroy(std::shared_ptr<GraphicsObject>& obj) {
  objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
}

int Diagram::numOfShapes() const { return objs.size(); }
void Diagram::clear() { objs.clear(); }