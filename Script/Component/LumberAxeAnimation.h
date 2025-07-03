#pragma once

#include "LumberAxeComponent.h"
#include "Components/Component.h"
#include "Components/Animator.h"
#include "Objects/BaseObject/Object.h"

class LumberAxeAnimation : public Component
{
public:
    LumberAxeAnimation(const std::string& name = "LumberAxeAnimation") : Component(name) {};
    virtual ~LumberAxeAnimation() override = default;

    // Component 인터페이스
    virtual void Update() override;
    virtual void Render() override; // 렌더링은 필요 없으므로 빈 구현

private:
    // 필요에 따라 내부적으로 상태를 캐싱하는 멤버 변수를 추가할 수 있습니다.
};

