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
    colliders.erase(remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void CollisionManager::RemoveAllCollider()
{
    colliders.clear();
}

void CollisionManager::Update()
{
    // 모든 콜라이더에 대해 충돌 검사
    for (size_t i = 0; i < colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < colliders.size(); ++j)
        {
            const auto& colliderA = colliders[i];
            const auto& colliderB = colliders[j];

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

std::vector<GameObject*> CollisionManager::GetCollidingObjects(GameObject* obj) const
{
    std::vector<GameObject*> result;
    if (!obj) return result;

    // GameObject에서 Collider 인터페이스를 구현하는 모든 컴포넌트를 가져옵니다.
    std::vector<Collider*> myColliders = obj->GetAllComponentsByInterface<Collider>();
    if (myColliders.empty()) return result;

    // CollisionManager에 등록된 모든 Collider와 비교합니다.
    for (const auto& otherCollider : colliders)
    {
        // 자기 자신은 제외
        if (otherCollider->GetOwner() == obj) continue;

        // obj의 모든 Collider와 충돌하는지 검사합니다.
        for (Collider* myCol : myColliders)
        {
            if (myCol->IsColliding(otherCollider.get()))
            {
                // 결과에 아직 추가되지 않았다면 추가합니다.
                if (find(result.begin(), result.end(), otherCollider->GetOwner()) == result.end())
                {
                    result.push_back(otherCollider->GetOwner());
                }
                break; // 한 Collider에서 충돌이 발견되면 다른 Collider는 검사할 필요 없음.
            }
        }
    }
    return result;
}