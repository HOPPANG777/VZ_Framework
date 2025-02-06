#pragma once

class Collider : public Component
{
public:
    enum class Type
    {
        Box,
        Circle,
    };

    Collider(const string& name, Type type) : Component(name), type(type) {}
    virtual ~Collider() = default;

    Type GetType() const { return type; }

    void SetCollisionCallback(function<void(Collider*)> callback)
    {
        collisionCallback = callback;
    }

    virtual bool IsColliding(const Collider* other) const = 0;

    virtual void OnCollision(Collider* other)
    {
        if (collisionCallback) collisionCallback(other);
    }

protected:
    Type type;
    function<void(Collider*)> collisionCallback;
};