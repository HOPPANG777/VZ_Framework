#pragma once
#include "Framework.h"

class Component;

class Entity
{
public:
    using ComponentMap = std::unordered_map<std::string, std::shared_ptr<Component>>;

    explicit Entity(int id) : id(id) {}

    template<typename T>
    std::shared_ptr<T> GetComponent(const std::string& name)
    {
        if (!components.count(name)) return nullptr;
        return std::dynamic_pointer_cast<T>(components[name]);
    }

    void AddComponent(const std::string& name, std::shared_ptr<Component> component)
    {
        components[name] = component;
    }

    int GetId() const { return id; }

private:
    int id;
    ComponentMap components;
};