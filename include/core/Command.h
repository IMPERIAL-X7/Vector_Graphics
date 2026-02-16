#pragma once
#include <deque>

#include "core/Diagram.h"
#include "core/GraphicsObject.h"

struct command_info {
  // Command Types:
  // 0: Creation (Undo -> Destroy)
  // 1: Destruction (Undo -> Create)
  // 2: Move
  int type;
  std::shared_ptr<GraphicsObject> object;
  QLineF displacement;  // For move operations
  command_info(int type, std::shared_ptr<GraphicsObject> object,
               QLineF displacement) {
    this->type = type;  // 0, 1, or 2
    this->object = object;
    this->displacement = displacement;
  }
};

class Command {
 public:
  Command();
  void undo();
  void redo();
  void undo_push(const command_info& infos);
  void redo_push(const command_info& infos);
  void setDiagram_cmd(Diagram* di);
  void clearRedoStack();

 private:
  std::deque<command_info> undo_stack;
  std::deque<command_info> redo_stack;
  Diagram* diagram;
};