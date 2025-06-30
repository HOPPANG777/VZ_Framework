#pragma once
#include "Components/Entity.h"
#include "Framework.h"

class EntityManager
{
public:
    std::shared_ptr<Entity> CreateEntity()
    {
        auto entity = std::make_shared<Entity>(nextId++);
        entities.push_back(entity);
        return entity;
    }

    std::vector<std::shared_ptr<Entity>>& GetEntities() { return entities; }

private:
    std::vector<std::shared_ptr<Entity>> entities;
    int nextId = 0;
};