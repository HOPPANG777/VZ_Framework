#pragma once
#include <cmath>
#include "Collider.h"

class CircleCollider : public Collider
{
public:
    CircleCollider(const Vector3& center, float radius, const Color& color = GREEN) : Collider("CircleCollider", Type::Circle), center(center), radius(radius), color(color) {}
    ~CircleCollider() override = default;

public:
    // ComponentÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
    void Update() override;
    void Render() override;

public:
    void SetCenter(const Vector3& newCenter) { center = newCenter; }
    void SetRadius(float newRadius) { radius = newRadius; }

public:
    Vector3 GetCenter() const { return center; }
    float GetRadius() const { return radius; }

public:
    bool IsColliding(const Collider* other) const override
    {
        if (other->GetType() == Type::Circle)
        {
            const CircleCollider* otherCircle = static_cast<const CircleCollider*>(other);
            return IsCircleColliding(otherCircle);
        }
        return false;
    }

private:
    bool IsCircleColliding(const CircleCollider* other) const
    {
        float dx = center.x - other->center.x;
        float dy = center.y - other->center.y;
        float distance = sqrt(dx * dx + dy * dy);

        return distance < (radius + other->radius);
    }

    Vector3 center;
    float radius;
    Color color;

};