#pragma once
#include "Interfaces/IRenderer.h"
//#include <memory>

// 렌더러 생성 및 접근을 관리하는 매니저 클래스입니다.
class RendererManager
{
public:
    // 렌더러 생성
    static void Create(RenderAPI api);
    // 렌더러 객체 반환
    static IRenderer* Get();

private:
    static IRenderer* renderer; // 현재 사용 중인 렌더러
};
