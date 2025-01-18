#include "Framework.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager() {}

CollisionManager::~CollisionManager() {}

void CollisionManager::AddCollider(std::shared_ptr<Collider> collider)
{
    colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(std::shared_ptr<Collider> collider)
{
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void CollisionManager::Update()
{
    // 모든 콜라이더에 대해 충돌 검사
    for (size_t i = 0; i < colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < colliders.size(); ++j)
        {
            auto colliderA = colliders[i];
            auto colliderB = colliders[j];

            if (colliderA->IsColliding(colliderB.get()))
            {
                colliderA->OnCollision(colliderB.get());
                colliderB->OnCollision(colliderA.get());
            }
        }
    }
}

void CollisionManager::RenderDebug()
{
    if (isColliderDebug == false)
    {
        isColliderDebug = true;
    }
    else
    {
        isColliderDebug = false;
    }
}

//// 디버그 렌더링 (콜라이더를 시각적으로 표시)
//for (auto& collider : colliders)
//{
//    //collider->RenderDebug();
//}
