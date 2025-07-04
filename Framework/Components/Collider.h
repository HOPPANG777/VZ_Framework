#pragma once

#include "Component.h"

class Collider : public Component
{
public:
    enum class Type
    {
        Box,
        Circle,
    };

    Collider(const std::string& name, Type type) : Component(name), type(type) {}
    virtual ~Collider() = default;

    Type GetType() const { return type; }

    void SetCollision(std::function<void(Collider*)> setCollision)
    {
        collisionCallback = setCollision;
    }

    virtual bool IsColliding(const Collider* other) const = 0;

    virtual void OnCollision(Collider* other)
    {
        if (collisionCallback) collisionCallback(other);
    }

protected:
    Type type;
    std::function<void(Collider*)> collisionCallback;
};