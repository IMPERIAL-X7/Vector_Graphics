#include "ui/Canvas.h"

#include "core/ShapeMaker.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  setFixedSize(600, 400);
  QPalette pal = palette();
  pal.setColor(QPalette::Window, Qt::white);
  setPalette(pal);
  setAutoFillBackground(true);
}
Canvas::~Canvas() {}

void Canvas::setcurrStroke(QColor c) { currStroke = c; }
void Canvas::setcurrFill(QColor c) { currFill = c; }
void Canvas::setStrokeWidth(double d) { currStrokeWidth = d; }

void Canvas::setDiagram_Cmd(Diagram* d, Command* cmd) {
  diagram = d;
  commandPlate = cmd;
  update();  // update tells Qt smthng had changed please redraw.
}

// std::shared_ptr<GraphicsObject> Canvas::getCanvas() const { return currShape;
// }
// void Canvas::resizeCanvas(int w, int h) {
//   setFixedSize(w, h);
//   update();
// }

void Canvas::setTool(Tool t) { currentTool = t; }

// Search for a shape at the given point (Hit Detection)
// Iterates in reverse to find the topmost object first
std::shared_ptr<GraphicsObject> Canvas::search(const QPointF& p_) {
  if (!diagram) return nullptr;
  const auto& objs = diagram->objects();

  for (auto it = objs.rbegin(); it != objs.rend(); ++it) {
    if ((*it)->boundingBox().contains(p_)) return *it;
  }
  return nullptr;
}

// Main drawing function, called automatically by Qt when update() is triggered
void Canvas::paintEvent(QPaintEvent*) {
  if (!diagram) return;

  QPainter p(this);
  p.fillRect(rect(), Qt::white);            // Clear background
  p.setRenderHint(QPainter::Antialiasing);  // Smooth edges
  // Antialiasing to smoothen the curve, remove staircase effect.
  for (const auto& obj : diagram->objects()) {
    obj->draw(p);
  }

  if (currentTool == Tool::Select) {
    if (!currShape) return;
    p.setPen(QPen(Qt::red, 2, Qt::DashDotDotLine));
    p.drawRect(currShape->boundingBox());
  }

  if (dragging) {
    if (currentTool == Tool::None) return;
    double resizeFactor = 1.0;
    QRectF box(startPos, currentPos);
    box = box.normalized();

    if (currentTool == Tool::Resize || currentTool == Tool::Move) {
      if (!currShape) return;
      double distCenter =
          QLineF(currShape->boundingBox().center(), justBefore).length();
      double distFact =
          QLineF(currShape->boundingBox().center(), currentPos).length();
      double factor = (distFact / distCenter);
      if (factor > 1.01 || factor < 0.99) {
        if (currentTool == Tool::Resize)
          currShape->resize(factor);
        else if (currentTool == Tool::Move)
          currShape->move(QLineF(justBefore, currentPos));
        justBefore = currentPos;
      }
      box = currShape->boundingBox();
    }

    p.setPen(QPen(Qt::red, 2, Qt::DashDotDotLine));
    p.setBrush(Qt::NoBrush);
    p.drawRect(box);
    if (currentTool == Tool::Freehand) {
      currFreehand->draw(p);
      return;
    }
    if (currentTool != Tool::Resize && currentTool != Tool::Move) {
      auto previewShape_ = ShapeMaker::create(currentTool, currStroke, currFill,
                                              currStrokeWidth);
      previewShape_->setBoundingBox(startPos, currentPos);
      previewShape_->draw(p);
    }
  }
}

void Canvas::copy() {
  copiedShape = currShape->clone();
  update();
}

void Canvas::cut() {
  copy();
  diagram->destroy(currShape);
  update();
}

void Canvas::paste() {
  copiedShape->move(QLineF(0.0, 0.0, 5.0, 5.0));
  diagram->add(copiedShape);
  update();
}