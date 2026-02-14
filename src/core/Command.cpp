#include "core/Command.h"

Command::Command() {};

void Command::setDiagram_cmd(Diagram* di)
{
    diagram = di;
}

void Command::undo_push(const command_info& infos)
{
    undo_stack.push_back(infos);
    if(undo_stack.size() > 15) undo_stack.pop_front();
}

void Command::redo_push(const command_info& infos)
{
    redo_stack.push_back(infos);
    if(redo_stack.size() > 15) redo_stack.pop_front();
}

void Command::undo()
{
    if(undo_stack.empty()) return;
    auto cmd = undo_stack.back(); undo_stack.pop_back();
    if(cmd.type == 0) 
    {
        redo_stack.push_back({1, cmd.object, QLineF(0.0, 0.0, 0.0, 0.0)});
        diagram->destroy(cmd.object);
    }
    else if(cmd.type == 1)
    {
        redo_stack.push_back({0, cmd.object, QLineF(0.0, 0.0, 0.0, 0.0)});
        diagram->add(cmd.object);
    }
    else if(cmd.type == 2)
    {
        QLineF rev(0.0, 0.0, -cmd.displacement.dx(), -cmd.displacement.dy());
        cmd.object->move(rev);
        redo_stack.push_back({2, cmd.object, rev});
    }
    else return;
}

void Command::redo()
{
    if(redo_stack.empty()) return;
    auto cmd = redo_stack.back(); redo_stack.pop_back();
    if(cmd.type == 0) 
    {
        undo_stack.push_back({1, cmd.object, QLineF(0.0, 0.0, 0.0, 0.0)});
        diagram->destroy(cmd.object);
    }
    else if(cmd.type == 1)
    {
        undo_stack.push_back({0, cmd.object, QLineF(0.0, 0.0, 0.0, 0.0)});
        diagram->add(cmd.object);
    }
    else if(cmd.type == 2)
    {
        QLineF rev(0.0, 0.0, -cmd.displacement.dx(), -cmd.displacement.dy());
        cmd.object->move(rev);
        undo_stack.push_back({2, cmd.object, rev});
    }
    else return;
}

void Command::clearRedoStack()
{
    redo_stack.clear();
}