/*
    include basically copy pastes your file content,
    so to prevent multiple time definition of a class
    we use pragma once to limit it,
    earlier ifndef/define was used.
*/

#pragma once 
#include "GraphicsObject.h"
#include<vector>

class Diagram
{
public:
    void add(const std::shared_ptr<GraphicsObject>& obj);
    const std::vector<std::shared_ptr<GraphicsObject>>& objects() const;
    void clear();
private:
    std::vector<std::shared_ptr<GraphicsObject>> objs; //remember to add _ later
};