#pragma once

#include "Object/PlayerUnit.h"

class WindZone : public Component
{
public:
    // regionMin, regionMax: 바람이 적용될 영역, windDirection: 바람의 방향 (정규화되어 있어야 함)
    // intensity: 바람 세기 (속도 단위, 예: 픽셀/초 또는 게임 단위)
    WindZone(const string& name,
        const Vector3& regionMin,
        const Vector3& regionMax,
        const Vector3& windDirection,
        float intensity)
        : Component(name),
        mRegionMin(regionMin),
        mRegionMax(regionMax),
        mWindDirection(windDirection),
        mIntensity(intensity)
    {
        mWindDirection.Normalize();
    }

    virtual ~WindZone() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {}  // 렌더링은 필요하지 않은 경우 빈 구현

private:
    Vector3 mRegionMin;
    Vector3 mRegionMax;
    Vector3 mWindDirection;
    float mIntensity;
};

