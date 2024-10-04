#include "Framework.h"
#include "Graphics.h"

Graphics::Graphics()
{
	Init();
}

Graphics::~Graphics() 
{
	
}

void Graphics::Init()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

    desc.BufferDesc.Width = 0;
    desc.BufferDesc.Height = 0;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    desc.BufferCount = 1;
    desc.OutputWindow = gHandle;
    desc.Windowed = true;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels.data(),
        UINT(featureLevels.size()),
        D3D11_SDK_VERSION,
        &desc,
        &swapChain,
        &device,
        nullptr,
        &deviceContext
    );

    CHECK(hr);  // HRESULT가 성공했는지 확인.

    CreateBackBuffer();
}

void Graphics::CreateBackBuffer()
{
    // 우리가 바꿀값만 제대로 넣으면 된다.
    HRESULT hr = swapChain->ResizeBuffers
    (
        0,
        UINT(gWinWidth),
        UINT(gWinHeight),
        DXGI_FORMAT_UNKNOWN,
        0
    );

    CHECK(hr);

    // ComPtr -> 스마트 포인터 -> 로포인터
    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    // backBuffer.GetAddressOf 
    // &를 쓸때 이걸 쓸놈이 무언가 담겨있지 않은지 확인해야 하고 
    // 만약 지우면 안되는데 &를 써야 한다면 이걸 쓰면 된다.
    // surface 그려질 표면
    hr = swapChain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backBuffer
    );

    CHECK(hr);

    // 파생클래스의 주소를 가질수있다 이걸 업캐스팅이라 부른다.
    hr = device->CreateRenderTargetView
    (
        backBuffer.Get(),
        nullptr,
        &rtv
    );

    CHECK(hr);

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = gWinWidth;
    viewport.Height = gWinHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
}

void Graphics::Begin()
{
    deviceContext->RSSetViewports(1, &viewport);
    deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
    deviceContext->ClearRenderTargetView(rtv.Get(), clearColor);
}

void Graphics::End()
{
    HRESULT hr = swapChain->Present(1, 0);
    CHECK(hr);
}
