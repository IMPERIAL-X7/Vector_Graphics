#include "core/Diagram.h"

void Diagram:: add(std::shared_ptr<GraphicsObject>& obj)
{
    undo_push_(std::make_pair(obj, 0));
    objs.push_back(obj);
}

const std::vector<std::shared_ptr<GraphicsObject>>& Diagram:: objects() const
{
    return objs;
}

void Diagram::destroy(std::shared_ptr<GraphicsObject>& obj)
{
    undo_push_(std::make_pair(obj, 1));
    objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
    //remove pushes the objects matching obj to the end of vector list and return an iterator,
    //then erase deletes all the elements from that iterator till the end.
}

void Diagram::undo_push_(const std::pair<std::shared_ptr<GraphicsObject>, int>& p)
{
    undo_stack.push_back(p);
    if(undo_stack.size() == 15) undo_stack.pop_front();
}

void Diagram::redo_push_(const std::pair<std::shared_ptr<GraphicsObject>, int>& p)
{
    redo_stack.push_back(p);
    if(redo_stack.size() == 15) redo_stack.pop_front();
}

void Diagram::undo()
{
    auto p = undo_stack.back(); undo_stack.pop_back();
    redo_push_(p);
    if(p.second == 0) objs.erase(std::remove(objs.begin(), objs.end(), p.first), objs.end());
    else if(p.second == 1) objs.push_back(p.first);
}

void Diagram::redo()
{
    auto p = redo_stack.back(); redo_stack.pop_back();
    undo_push_(p);
    if(p.second==0) objs.push_back(p.first);
    else if(p.second==1) objs.erase(std::remove(objs.begin(), objs.end(), p.first), objs.end());
}

void Diagram::clear()
{
    objs.clear();
}