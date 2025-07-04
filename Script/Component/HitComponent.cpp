#include "stdafx.h"
#include "HitComponent.h"

void HitComponent::Activate(float dur, const Color& flash)
{
    flashColor = flash;
    duration = dur;
    timer = 0.0f;
    active = true;

    // 소유 객체가 IColorable 인터페이스를 구현한다고 가정합니다.
    IColorable* colorable = dynamic_cast<IColorable*>(GetOwner());
    if (colorable)
    {
        originalColor = colorable->GetColor(); // 현재 색상 저장
        colorable->SetColor(flashColor);       // 즉시 빨간색으로 변경
    }
}

void HitComponent::Update()
{
    if (!active) return;

    // 효과 진행 시간 증가
    timer += Time::Get()->GetDeltaTime();
    float t = timer / duration;
    if (t >= 1.0f)
    {
        t = 1.0f;
        active = false;
    }

    // 선형 보간을 통해 flashColor에서 originalColor로 전환
    float r = flashColor.R() + (originalColor.R() - flashColor.R()) * t;
    float g = flashColor.G() + (originalColor.G() - flashColor.G()) * t;
    float b = flashColor.B() + (originalColor.B() - flashColor.B()) * t;
    float a = flashColor.A() + (originalColor.A() - flashColor.A()) * t;

    Color currentColor(r, g, b, a);

    IColorable* colorable = dynamic_cast<IColorable*>(GetOwner());
    if (colorable)
    {
        colorable->SetColor(currentColor);
    }
}