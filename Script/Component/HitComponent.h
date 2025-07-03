#pragma once

#include "Components/Component.h"
#include "Interfaces/IColorable.h"
#include "Objects/BaseObject/Object.h"

class HitComponent : public Component
{
public:
    HitComponent(const string& name = "HitComponent") : Component(name), active(false), timer(0.0f), duration(0.0f), originalColor(WHITE), flashColor(Color(1, 0, 0, 1))
    {

    }
    virtual ~HitComponent() override = default;

    virtual void Update() override;
    virtual void Render() override {} // Render는 필요 없으므로 빈 구현

    // 피격 효과를 활성화합니다.
    // flashColor: 피격 시 표시할 색상 (기본 빨간색)
    // duration: 효과 지속 시간 (초)
    void Activate(float duration = 1.0f, const Color& flashColor = Color(1, 0, 0, 1));

private:
    bool active;         // 현재 피격 효과가 활성화 되었는지 여부
    float timer;         // 경과 시간
    float duration;      // 전체 효과 지속 시간

    Color originalColor; // 원래 색상 (피격 전)
    Color flashColor;    // 피격 시 색상

};

