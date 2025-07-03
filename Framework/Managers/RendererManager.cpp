#include "Framework.h"
#include "RendererManager.h"
#include "Systems/Graphics.h"

std::unique_ptr<IRenderer> RendererManager::renderer = nullptr;

// 렌더러 생성
void RendererManager::Create(RenderAPI api)
{
    switch (api)
    {
    case RenderAPI::DirectX11:
        renderer = std::make_unique<Graphics>();
        break;
    default:
        renderer = nullptr;
        break;
    }
}

// 렌더러 접근
IRenderer* RendererManager::Get()
{
    return renderer.get();
}
