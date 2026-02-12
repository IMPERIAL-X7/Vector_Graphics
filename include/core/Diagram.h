/*
    include basically copy pastes your file content,
    so to prevent multiple time definition of a class
    we use pragma once to limit it,
    earlier ifndef/define was used.
*/

#pragma once 
#include "GraphicsObject.h"
#include<vector>
#include<deque>

class Diagram
{
public:
    void add(std::shared_ptr<GraphicsObject>& obj);
    const std::vector<std::shared_ptr<GraphicsObject>>& objects() const;
    void destroy(std::shared_ptr<GraphicsObject>& obj);
    void undo();
    void redo();
    void clear();
private:
    std::vector<std::shared_ptr<GraphicsObject>> objs; //remember to add _ later
    std::deque<std::pair<std::shared_ptr<GraphicsObject>, int>>undo_stack;
    std::deque<std::pair<std::shared_ptr<GraphicsObject>, int>>redo_stack;
    void undo_push_(const std::pair<std::shared_ptr<GraphicsObject>, int>& p);
    void redo_push_(const std::pair<std::shared_ptr<GraphicsObject>, int>& p);
};

/*
    no. -> lastAction
    0 -> add
    1 -> delete
    2 -> resize
    3 -> move
*/