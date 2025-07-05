#pragma once

class SolidCollider : public Collider
{
public:
    // offset: 소유 객체의 기준 위치에서의 상대 좌표
    // size: 충돌 영역 크기 (AABB)
    SolidCollider(const string& name, const Vector3& offset, const Vector3& size, const Color& color = WHITE) : Collider(name, Type::Box), offset(offset), size(size), color(color)
    {

    }
    virtual ~SolidCollider() override = default;

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    // 다른 Collider와 충돌했을 때 호출되어 위치를 조정하는 함수
    void ResolveCollision(Collider* other);

public:
    // AABB 정보 계산을 위한 헬퍼 함수들
    Vector3 GetCenter() const;
    Vector3 GetHalfSize() const;

private:
    Vector3 offset; // 소유 객체 기준 상대 위치
    Vector3 size;   // 충돌 영역 크기
    Color color;
};
