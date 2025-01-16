#pragma once

class CollisionManager
{
	DECLARE_SINGLETON(CollisionManager);

public:
    void AddCollider(std::shared_ptr<Collider> collider);
    void RemoveCollider(std::shared_ptr<Collider> collider);
    void Update();
    void RenderDebug();

public:
    bool GetColliderDebug() const { return isColliderDebug; }

private:
    std::vector<std::shared_ptr<Collider>> colliders; // 관리하는 콜라이더 리스트

    bool isColliderDebug = false;
};