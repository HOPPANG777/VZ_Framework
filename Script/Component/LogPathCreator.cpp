#include "stdafx.h"
#include "LogPathCreator.h"

LogPathCreator::LogPathCreator(const string& name)
    : Component(name), collisionCount(0), transformed(false), newColliderSize(500, 100, 0)
{

}

void LogPathCreator::Update()
{

}

void LogPathCreator::Render()
{

}

void LogPathCreator::OnAxeCollision()
{
    // 이미 변환된 경우에는 더 이상 처리하지 않음
    if (transformed) return;

    // 충돌 횟수를 증가
    collisionCount++;

	auto owner = GetOwner();

    // 피격 효과 활성화: HitComponent가 있다면 Activate() 호출
    owner->GetComponent<HitComponent>("HitComponent")->Activate();

    // 3회 이상 충돌하면 변환 처리
    if (collisionCount >= 3)
    {
        // log 오브젝트(Owner)의 회전을 오른쪽으로 90도 변경
        // (예: 90도)
        owner->SetRotation(-90);

        owner->SetPosition(owner->GetPosition() + Vector3(300, -280, 0));

        // 기존 BoxCollider 제거
        owner->RemoveComponent("BoxCollider");

        // 새로운 BoxCollider 생성 (크기: 500,300,0)
        auto newCollider = make_shared<BoxCollider>(Vector3(0, 0, 0), newColliderSize, GREEN);
        owner->AddComponent(newCollider);

        // CollisionManager에 새 콜라이더 등록
        COLLISION->AddCollider(newCollider);

        transformed = true;
    }
}

// 예시 콜백 (로그 오브젝트의 BoxCollider에 연결된 OnCollision)
void LogPathCreator::OnLogCollision(Collider* other)
{
    // 상대 오브젝트가 LumberAxe인지 체크 (예: 이름에 "LumberAxe" 포함)
    if (other->GetOwner()->GetName().find("LumberAxe") != string::npos)
    {
        // log[0]의 LogPathCreator 컴포넌트를 찾아 OnAxeCollision() 호출
        auto logPathComp = GetOwner()->GetComponent<LogPathCreator>("LogPathCreator");
        if (logPathComp) logPathComp->OnAxeCollision();
    }
}