#pragma once

class BoxCollider : public Collider
{
public:
    BoxCollider(const string& name, const Vector3& center, const Vector3& size, const Color& color = GREEN) : Collider(name, Type::Box), center(center), size(size), color(color) {}
    ~BoxCollider() override = default;

    // Component��(��) ���� ��ӵ�
    void Update() override;
    void Render() override;

    void SetCenter(const Vector3& newCenter) { center = newCenter; }
    void SetSize(const Vector3& newSize) { size = newSize; }

    Vector3 GetCenter() const { return center; }
    Vector3 GetSize() const { return size; }

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
        return (abs(center.x - other->center.x) * 2 < (size.x + other->size.x)) &&
            (abs(center.y - other->center.y) * 2 < (size.y + other->size.y));
    }

    Vector3 center;
    Vector3 size;
    Color color;
};


