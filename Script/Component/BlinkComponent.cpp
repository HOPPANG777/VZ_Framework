#include "stdafx.h"
#include "BlinkComponent.h"

void BlinkComponent::Activate(float dur, float inter)
{
    duration = dur;
    interval = inter;
    timer = 0.0f;
    tick = 0.0f;
    active = true;
    visible = true;
    IColorable* colorable = dynamic_cast<IColorable*>(GetOwner());
    if (colorable) originalColor = colorable->GetColor();
}

void BlinkComponent::Update()
{
    if (!active) return;

    timer += DELTA;
    tick += DELTA;

    if (tick >= interval)
    {
        tick = 0.0f;
        visible = !visible;
        IColorable* colorable = dynamic_cast<IColorable*>(GetOwner());
        if (colorable)
        {
            Color c = originalColor;
            if (!visible) c.A(0.0f);
            colorable->SetColor(c);
        }
    }

    if (timer >= duration)
    {
        active = false;
        IColorable* colorable = dynamic_cast<IColorable*>(GetOwner());
        if (colorable)
            colorable->SetColor(originalColor);
    }
}
