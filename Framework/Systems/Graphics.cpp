#include "Framework.h"
#include "Graphics.h"

Graphics::Graphics()
{
    EnumerateAdapters();
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

    if (bVsync)
    {
        desc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
        desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
    }
    else
    {
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
    }

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

    UINT maxVRam = 0;
    for (UINT i = 0; i < adapterInfos.size(); i++)
    {
        // adapterInfos[i]는 unique_ptr
        auto& info = adapterInfos[i];
        UINT64 dedicatedMem = info->adapterDesc.DedicatedVideoMemory;

        if (dedicatedMem > maxVRam)
        {
            selectedAdapterIndex = i;
            maxVRam = (UINT)dedicatedMem;
        }
    }

    // 출력해보기
    for (size_t i = 0; i < adapterInfos.size(); i++)
    {
        wcout << L"GPU Name : " << adapterInfos[i]->adapterDesc.Description << endl;
        cout << "VRAM : " << adapterInfos[i]->adapterDesc.DedicatedVideoMemory << endl;
        cout << endl;
    }

    wcout << L"Selected GPU Name : " << adapterInfos[selectedAdapterIndex]->adapterDesc.Description << endl;

    ComPtr<IDXGIAdapter1> chosenAdapter = adapterInfos[selectedAdapterIndex]->adapter;

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        chosenAdapter.Get(),//nullptr,
        D3D_DRIVER_TYPE_UNKNOWN,//D3D_DRIVER_TYPE_WARP,
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

    CreateDirect2D();

    CreateDirectWrite();
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

    // Direct2D 그리기 시작
    d2DeviceContext->BeginDraw();
}

void Graphics::End()
{
    HRESULT D2hr = d2DeviceContext->EndDraw();
    CHECK(D2hr);

    HRESULT hr = swapChain->Present(bVsync == true ? 1 : 0, 0);
    CHECK(hr);
}

void Graphics::GUI()
{
    static bool bOpen = true;
    ImGui::SetNextWindowPos({ 0, 15 });
    ImGui::SetNextWindowSize(ImVec2(200, 30)); // 위젯 창 크기 설정
    ImGui::Begin
    (
        "Vstnc",
        &bOpen,
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar
    );
    {
        ImGui::Checkbox("##Vsync", &bVsync);
    }
    ImGui::End();
}

void Graphics::EnumerateAdapters()
{
    ComPtr<IDXGIFactory1> factory;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
        return;

    UINT index = 0;
    while (true)
    {
        ComPtr<IDXGIAdapter1> adapter = nullptr;
        HRESULT hr = factory->EnumAdapters1(index, &adapter);
        if (hr == DXGI_ERROR_NOT_FOUND)
            break;
        assert(SUCCEEDED(hr));

        auto adapterInfo = make_unique<D3DEnumAdapterInfo>();
        adapterInfo->adapterOrdinal = index;

        // 기수 / 서수
        // 일이삼사 첫번째 두번째
        adapter->GetDesc1(&adapterInfo->adapterDesc);
        adapterInfo->adapter = adapter;

        EnumerateAdapterOutput(adapterInfo.get());
        adapterInfos.push_back(move(adapterInfo));

        index++;
    }
}

bool Graphics::EnumerateAdapterOutput(D3DEnumAdapterInfo* adapterInfo)
{
    ComPtr<IDXGIOutput> output;
    HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);

    if (DXGI_ERROR_NOT_FOUND == hr) return false;
    assert(SUCCEEDED(hr));

    auto outputInfo = make_shared<D3DEnumOutputInfo>();

    output->GetDesc(&outputInfo->outputDesc);
    outputInfo->output = output;

    UINT numModes = 0;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    assert(SUCCEEDED(hr));

    unique_ptr<DXGI_MODE_DESC[]> displayModes = make_unique<DXGI_MODE_DESC[]>(numModes);

    hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes.get());
    assert(SUCCEEDED(hr));

    for (UINT i = 0; i < numModes; i++)
    {
        bool bCheck = true;
        bCheck &= displayModes[i].Width == WIN_DEFAULT_WIDTH;
        bCheck &= displayModes[i].Height == WIN_DEFAULT_HEIGHT;

        if (bCheck == true)
        {
            outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
            outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
        }
    }

    adapterInfo->outputInfo = outputInfo;
    return true;
}

void Graphics::CreateDirect2D()
{
    // Direct2D Factory 생성 (버전 1 사용)
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), nullptr, &d2Factory);
    CHECK(hr);

    // DXGI Device 얻기: 기존 D3D11Device에서 IDXGIDevice 인터페이스 획득
    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    hr = device.As(&dxgiDevice);
    CHECK(hr);

    // Direct2D Device 생성
    hr = d2Factory->CreateDevice(dxgiDevice.Get(), &d2Device);  // 시발 왜 안되는데 대체 뭐가 문제야!!!!!!!!!
    CHECK(hr);

    // Direct2D Device Context 생성
    hr = d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2DeviceContext);
    CHECK(hr);

    // Swap Chain의 백버퍼를 Direct2D 비트맵으로 연결
    ComPtr<IDXGISurface> dxgiSurface;
    hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);
    CHECK(hr);

    // Direct2D 비트맵 프로퍼티 정의 (픽셀 포맷, DPI 등)
    D2D1_BITMAP_PROPERTIES1 bitmapProperties;
    bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    bitmapProperties.dpiX = 96.0f;
    bitmapProperties.dpiY = 96.0f;
    bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

    // Direct2D Bitmap 생성 후 Device Context의 타깃으로 설정
    hr = d2DeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &d2Bitmap);
    CHECK(hr);

    d2DeviceContext->SetTarget(d2Bitmap.Get());
}

void Graphics::CreateDirectWrite()
{
    // IDWriteFactory 생성
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf()));
    CHECK(hr);
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo() {}

D3DEnumOutputInfo::~D3DEnumOutputInfo() {}

