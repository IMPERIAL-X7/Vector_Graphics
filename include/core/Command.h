#pragma once
#include "core/GraphicsObject.h"
#include "core/Diagram.h"
#include <deque>

struct command_info
{
    int type; std::shared_ptr<GraphicsObject> object; QLineF displacement;
    command_info(int type, std::shared_ptr<GraphicsObject> object, QLineF displacement) {
        this->type = type; this->object = object; this->displacement = displacement;
    }
};

class Command
{
public:
    Command();
    void undo();
    void redo();
    void undo_push(const command_info& infos);
    void redo_push(const command_info& infos);
    void setDiagram_cmd(Diagram* di);
    void clearRedoStack();
private:
    std::deque<command_info>undo_stack;
    std::deque<command_info>redo_stack;
    Diagram* diagram;
};