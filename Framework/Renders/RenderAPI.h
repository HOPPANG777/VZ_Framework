#pragma once

// 사용할 렌더링 API를 나타내는 열거형 입니다.
// 추후 OpenGL, Vulkan 등을 추가할 수 있습니다.
enum class RenderAPI
{
    DirectX11, // 디렉트X11 사용
    OpenGL,    // 오픈GL 사용
    Vulkan     // 벌컨 사용
};
