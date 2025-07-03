#pragma once

#include "Systems/Graphics.h"

class Gizmo
{
    DECLARE_SINGLETON(Gizmo);

public:
    // 도형 그리기
    static void DrawBox(const Vector3& center, const Vector3& size, const Color& color, float thickness = 1.0f);
    static void DrawCircle(const Vector3& center, float radius, const Color& color, float thickness = 1.0f);
    static void DrawLine(const Vector3& start, const Vector3& end, const Color& color, float thickness = 1.0f);
};

