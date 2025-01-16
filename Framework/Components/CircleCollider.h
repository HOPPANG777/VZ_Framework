#pragma once

class CircleCollider : public Collider
{
public:
    CircleCollider(const string& name, const Vector3& center, float radius, const Color& color = GREEN) : Collider(name, Type::Circle), center(center), radius(radius), color(color) {}
    ~CircleCollider() override = default;

    // Component을(를) 통해 상속됨
    void Update() override;
    void Render() override;

    void SetCenter(const Vector3& newCenter) { center = newCenter; }
    void SetRadius(float newRadius) { radius = newRadius; }

    Vector3 GetCenter() const { return center; }
    float GetRadius() const { return radius; }

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