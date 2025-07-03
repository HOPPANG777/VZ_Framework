#pragma once

// 플랫폼 별 창 관리 인터페이스입니다.
class IWindow
{
public:
    virtual ~IWindow() = default;

    // 윈도우 실행 루프 진입
    virtual unsigned int Run() = 0;
};
