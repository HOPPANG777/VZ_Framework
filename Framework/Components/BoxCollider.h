#pragma once

class BoxCollider : public Collider
{
public:
    BoxCollider(const Vector3& center, const Vector3& size, const Color& color = GREEN) : Collider("BoxCollider", Type::Box), center(center), size(size), color(color) 
    {
        SetExtents(size);
    }
    ~BoxCollider() override = default;

public:
    // Component 상속
    void Update() override;
    void Render() override;
    void UpdateEdge() { edge.left = center.x - extents.x; edge.right = center.x + extents.x; edge.top = center.y + extents.y; edge.bottom = center.y - extents.y; }

public:
    void SetCenter(const Vector3& newCenter) { center = newCenter; }
    void SetSize(const Vector3& newSize) { size = newSize; }
	void SetExtents(const Vector3& size) { extents = size * 0.5f; }


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
        //return (abs(center.x - other->center.x) <= (size.x * 0.5f + other->size.x * 0.5f)) &&
        //    (abs(center.y - other->center.y) <= (size.y * 0.5f + other->size.y * 0.5f));

        // X축 분리 검사
        if (edge.left > other->edge.right || edge.right < other->edge.left) return false;

        // Y축 분리 검사
        if (edge.top < other->edge.bottom || edge.bottom > other->edge.top) return false;
        
        // 두 축이 모두 겹치면 충돌
        return true;
    }

    Vector3 center;
    Vector3 size;
	Vector3 extents;
    Color color;
    BoxEdge edge;

};


