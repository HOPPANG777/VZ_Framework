#pragma once

class BoxCollider : public Collider
{
public:
    BoxCollider(const Vector3& center, const Vector3& size, const Color& color = GREEN) : Collider("BoxCollider", Type::Box), center(center), size(size), color(color) {}
    ~BoxCollider() override = default;

public:
    // Component 상속
    void Update() override;
    void Render() override;

public:
    void SetCenter(const Vector3& newCenter) { center = newCenter; }
    void SetSize(const Vector3& newSize) { size = newSize; }

public:
    Vector3 GetCenter() const { return center; }
    Vector3 GetSize() const { return size; }

public:
    bool IsColliding(const Collider* other) const override
    {
        if (other->GetType() == Type::Box)
        {
            const BoxCollider* otherBox = static_cast<const BoxCollider*>(other);
            return IsBoxColliding(otherBox);
        }
        return false;
    }


private:
    bool IsBoxColliding(const BoxCollider* other) const
    {
        return (abs(center.x - other->center.x) <= (size.x * 0.5f + other->size.x * 0.5f)) &&
            (abs(center.y - other->center.y) <= (size.y * 0.5f + other->size.y * 0.5f));
    }

    Vector3 center;
    Vector3 size;
    Color color;

};


