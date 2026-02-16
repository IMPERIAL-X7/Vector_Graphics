#include <QInputDialog>
#include <QMouseEvent>
#include <QPainter>
#include <iostream>  // for debugging

#include "core/ShapeMaker.h"
#include "ui/Canvas.h"

void Canvas::mouseDoubleClickEvent(QMouseEvent* e) {
  // Double-click to start moving a shape
  currentTool = Tool::Move;
  currShape = search(e->position());

  // Prepare for drag operation
  update();
  dragging = true;
  startPos = e->position();
  justBefore = startPos;
  currentPos = startPos;
}

void Canvas::mousePressEvent(QMouseEvent* e) {
  // Right-click initiates resizing of the shape under cursor
  if (e->button() == Qt::RightButton) {
    currentTool = Tool::Resize;
    currShape = search(e->position());

    // Initialize drag state variables
    update();
    dragging = true;
    startPos = e->position();
    justBefore = startPos;
    currentPos = startPos;
    return;
  }

  // Left-click logic
  // If Select tool is active, just find and highlight the shape
  if (currentTool == Tool::Select) {
    currShape = search(e->position());
    update();
    return;
  }

  // Clear redo stack on new action
  commandPlate->clearRedoStack();

  // If Freehand tool, start a new freehand drawing
  if (currentTool == Tool::Freehand) {
    currFreehand = std::dynamic_pointer_cast<Freehand>(ShapeMaker::create(
        Tool::Freehand, currStroke, currFill, currStrokeWidth));
    currFreehand->addPoint(e->position());
  }

  // If resizing or moving, identify the target shape
  if (currentTool == Tool::Resize || currentTool == Tool::Move) {
    currShape = search(e->position());
  }

  dragging = true;
  std::cout << "Drag started" << std::endl;
  startPos = e->position();
  justBefore = startPos;
  currentPos = startPos;
}

void Canvas::mouseMoveEvent(QMouseEvent* e) {
  // Only process moves if a drag operation is active
  if (!dragging || !diagram) return;

  currentPos = e->position();

  // Update freehand drawing in real-time
  if (dragging && currentTool == Tool::Freehand) {
    currFreehand->addPoint(currentPos);
    update();
  }
  update();  // Trigger redraw for rubber-banding or movement
}

void Canvas::mouseReleaseEvent(QMouseEvent* e) {
  if (!dragging || !diagram) return;

  std::cout << "Shapes count: " << diagram->numOfShapes() << std::endl;
  dragging = false;
  update();
  currentPos = e->position();

  // Normalize creation box
  auto box_ = QRectF(startPos, currentPos);
  box_ = box_.normalized();

  // Ignore tiny accidental drags
  double area = box_.width() * box_.height();
  if (area < 3) return;

  // Finalize Freehand drawing
  if (currentTool == Tool::Freehand && currFreehand) {
    std::cout << "Finalizing freehand" << std::endl;
    auto obj = std::static_pointer_cast<GraphicsObject>(currFreehand);
    diagram->add(obj);

    // Record action for undo
    commandPlate->undo_push({0, currFreehand, QLineF()});

    currFreehand->setBoundingBox(startPos, currentPos);
    currFreehand = nullptr;
    startPos = currentPos;
    update();
    return;
  }

  if (currentTool == Tool::Resize) return;

  // Handle move completion
  if (currentTool == Tool::Move) {
    // Record move for undo
    commandPlate->undo_push({2, currShape, QLineF(startPos, currentPos)});
    return;
  }

  // Handle standard shape creation (Rect, Circle, etc.)
  auto shape =
      ShapeMaker::create(currentTool, currStroke, currFill, currStrokeWidth);
  if (!shape) return;

  shape->setBoundingBox(startPos, currentPos);

  if (currentTool == Tool::RoundRect) {
    bool ok;
    float radius = QInputDialog::getDouble(this, tr("Round Rect"),
                                           tr("Enter Corner Radius:"), 10, 0,
                                           10000, 1, &ok);
    if (ok) {
      auto rr = std::dynamic_pointer_cast<RoundRect>(shape);
      rr->setRadius(radius);
      std::cout << rr->viewRadius() << std::endl;
    } else {
      update();
      return;
    }
  } else if (currentTool == Tool::Text) {
    bool ok;
    QString qText =
        QInputDialog::getText(this, tr("Text Box"), tr("Enter text"),
                              QLineEdit::Normal, tr("write here"), &ok);
    if (ok) {
      auto textBox = std::dynamic_pointer_cast<Text>(shape);
      textBox->setText(qText);
    } else {
      update();
      return;
    }
  }
  diagram->add(shape);
  commandPlate->undo_push({0, shape, QLineF(startPos, currentPos)});
  if (currentTool == Tool::Circle) {
    auto s = std::dynamic_pointer_cast<Circle>(shape);
    s->resizeBoundingBox();
  }
  if (currentTool == Tool::Hexagon) {
    auto s = std::dynamic_pointer_cast<Hexagon>(shape);
    s->resizeBoundingBox();
  }
  update();
  startPos = currentPos;
}
