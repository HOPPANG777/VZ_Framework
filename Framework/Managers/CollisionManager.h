#pragma once
#include "../Components/Collider.h"
#include "../Objects/BaseObject/Object.h"

class CollisionManager
{
	DECLARE_SINGLETON(CollisionManager);

public:
    void AddCollider(shared_ptr<Collider> collider);
    void RemoveCollider(shared_ptr<Collider> collider);
    void RemoveAllCollider();
    void Update();
    void RenderDebug();

public:
    bool GetColliderDebug() const { return isColliderDebug; }
    // 추가: 입력된 오브젝트와 충돌 중인 모든 오브젝트를 반환하는 함수
    vector<Object*> GetCollidingObjects(Object* obj) const;

private:
    vector<shared_ptr<Collider>> colliders; // 관리하는 콜라이더 리스트
    bool isColliderDebug = false;

};