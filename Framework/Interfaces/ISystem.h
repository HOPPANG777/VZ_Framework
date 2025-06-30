#pragma once
#include "Framework.h"

class Entity;

class System
{
public:
    virtual ~System() = default;
    virtual void Update(std::vector<std::shared_ptr<Entity>>& entities) = 0;
};