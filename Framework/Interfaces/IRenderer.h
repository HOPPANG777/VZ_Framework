#pragma once
#include "Renders/RenderAPI.h"

// 렌더러 공통 인터페이스입니다.
// 다양한 그래픽 API 구현체가 해당 인터페이스를 상속받아 사용됩니다.
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // 렌더러 초기화
    virtual void Init() = 0;
    // 프레임 시작 처리
    virtual void Begin() = 0;
    // 프레임 종료 처리
    virtual void End() = 0;

    // 전체 화면 전환
    virtual void SetFullScreen(bool fullscreen) = 0;
    // 해상도 변경
    virtual void SetResolution(unsigned int width, unsigned int height) = 0;
};
