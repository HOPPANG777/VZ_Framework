#include "RendererManager.h"
#include "Systems/Graphics.h"

IRenderer* RendererManager::renderer = nullptr;

// 렌더러 생성
void RendererManager::Create(RenderAPI api)
{
    switch (api)
    {
    case RenderAPI::DirectX11:
        renderer = Graphics::Get();
        break;
    default:
        renderer = nullptr;
        break;
    }
}

// 렌더러 접근
IRenderer* RendererManager::Get()
{
    return renderer;
}
