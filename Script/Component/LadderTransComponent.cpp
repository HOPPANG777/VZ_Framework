#include "stdafx.h"
#include "LadderTransComponent.h"

void LadderTransComponent::OnAxeCollision(Collider* other)
{
    // 이미 변환된 경우에는 더 이상 처리하지 않음
    if (transformed) return;

    // 충돌 횟수를 증가
    collisionCount++;

    Object* owner = GetOwner();

    // 피격 효과 활성화: HitComponent가 있다면 Activate() 호출
    owner->GetComponent<HitComponent>("HitComponent")->Activate();

    // 3회 이상 충돌하면 변환 처리
    if (collisionCount >= 1)
    {
        //owner->SetPosition(owner->GetPosition() + Vector3(300, -280, 0));

        // 기존 BoxCollider 제거
        owner->RemoveComponent("BoxCollider");

        // 새로운 BoxCollider 생성 (크기: 500,300,0)
        const std::shared_ptr<BoxCollider>& newCollider = make_shared<BoxCollider>(Vector3(0, 0, 0), newColliderSize, GREEN);
        owner->AddComponent(newCollider);

        // CollisionManager에 새 콜라이더 등록
        COLLISION->AddCollider(newCollider);

        transformed = true;
    }

}

void LadderTransComponent::OnLadderCollision(Collider* other)
{
    // 상대 오브젝트가 LumberAxe인지 체크 (예: 이름에 "LumberAxe" 포함)
    if (other->GetOwner()->GetName().find("LumberAxe") != string::npos)
    {
        const std::shared_ptr<LadderComponent>& logPathComp = GetOwner()->GetComponent<LadderComponent>("LadderComponent");
        if (logPathComp) logPathComp->OnAxeCollision();
    }
}
