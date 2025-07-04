#include "Framework.h"
#include "Graphics.h"

Graphics::Graphics()
{
	EnumerateAdapters();
	Init();
}

Graphics::~Graphics()
{
	UnloadFont();
}

void Graphics::Init()
{
	LoadFont();

	CreateDirect3D();

	CreateBackBuffer();

	CreateDirect2D();

	CreateDirectWrite();
}

void Graphics::CreateBackBuffer()
{
	// 우리가 바꿀값만 제대로 넣으면 된다.
	//HRESULT hr = swapChain->ResizeBuffers
	//(
	//	0,
	//	UINT(gWinWidth),
	//	UINT(gWinHeight),
	//	DXGI_FORMAT_B8G8R8A8_UNORM,
	//	0
	//);


	// ResizeBuffers is handled in SetResolution
	HRESULT hr = S_OK;
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
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory))) return;

	UINT index = 0;
	while (true)
	{
		ComPtr<IDXGIAdapter1> adapter = nullptr;
		HRESULT hr = factory->EnumAdapters1(index, &adapter);
		if (hr == DXGI_ERROR_NOT_FOUND) break;
		assert(SUCCEEDED(hr));

		std::unique_ptr<D3DEnumAdapterInfo> adapterInfo = std::make_unique<D3DEnumAdapterInfo>();
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
	CHECK(hr);

	std::shared_ptr<D3DEnumOutputInfo> outputInfo = std::make_shared<D3DEnumOutputInfo>();

	output->GetDesc(&outputInfo->outputDesc);
	outputInfo->output = output;

	UINT numModes = 0;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	CHECK(hr);

	std::unique_ptr<DXGI_MODE_DESC[]> displayModes = std::make_unique<DXGI_MODE_DESC[]>(numModes);

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes.get());
	CHECK(hr);

	for (UINT i = 0; i < numModes; i++)
	{
		bool bCheck = true;
		bCheck &= displayModes[i].Width == Values::GetWinWidth();
		bCheck &= displayModes[i].Height == Values::GetWinHeight();

		if (bCheck == true)
		{
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}

	adapterInfo->outputInfo = outputInfo;
	return true;
}

void Graphics::CreateDirect3D()
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

	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	desc.BufferCount = 1;
	desc.OutputWindow = gHandle;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	std::vector<D3D_FEATURE_LEVEL> featureLevels
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT maxVRam = 0;
	for (UINT i = 0; i < adapterInfos.size(); i++)
	{
		// adapterInfos[i]는 std::unique_ptr
		std::unique_ptr<D3DEnumAdapterInfo>& info = adapterInfos[i];
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
		std::wcout << L"GPU Name : " << adapterInfos[i]->adapterDesc.Description << std::endl;
		std::cout << "VRAM : " << adapterInfos[i]->adapterDesc.DedicatedVideoMemory << std::endl;
		std::cout << std::endl;
	}

	std::wcout << L"Selected GPU Name : " << adapterInfos[selectedAdapterIndex]->adapterDesc.Description << std::endl;

	ComPtr<IDXGIAdapter1> chosenAdapter = adapterInfos[selectedAdapterIndex]->adapter;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // Direct2D와 호환
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // 디버그 레이어 활성화
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		chosenAdapter.Get(),//nullptr,
		D3D_DRIVER_TYPE_UNKNOWN,//D3D_DRIVER_TYPE_HARDWARE,//D3D_DRIVER_TYPE_WARP,
		nullptr,
		createDeviceFlags, // <- 여기에 Direct2D를 위한 디바이스를 여러개 써야해
		featureLevels.data(),
		UINT(featureLevels.size()),
		D3D11_SDK_VERSION,
		&desc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		deviceContext.GetAddressOf()
	);
	CHECK(hr);  // HRESULT가 성공했는지 확인.

	// 여기서 device가 생성되었으므로 InfoQueue를 설정할 수 있음
	/*
	ID3D11InfoQueue* pInfoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&pInfoQueue)))
	{
		// 숨기고 싶은 메시지 ID 목록
		D3D11_MESSAGE_ID hideMessages[] =
		{
			D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
			D3D11_MESSAGE_ID_IASETINDEXBUFFER_INVALIDBUFFER
		};

		D3D11_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = _countof(hideMessages);
		filter.DenyList.pIDList = hideMessages;

		// 메시지 필터 적용
		pInfoQueue->AddStorageFilterEntries(&filter);

		pInfoQueue->Release();
	}
	*/
}

void Graphics::CreateDirect2D()
{
	// Direct2D Factory 생성 (버전 1 사용)
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), nullptr, &d2Factory);
	CHECK(hr);

	// DXGI Device 얻기: 기존 D3D11Device에서 IDXGIDevice 인터페이스 획득
	hr = device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf()));
	CHECK(hr);

	// Direct2D Device 생성
	hr = d2Factory->CreateDevice(dxgiDevice.Get(), &d2Device);  // 시발 왜 안되는데 대체 뭐가 문제야!!!!!!!!!
	CHECK(hr);

	// Direct2D Device Context 생성
	hr = d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &d2DeviceContext);
	CHECK(hr);

	// Swap Chain의 백버퍼를 Direct2D 비트맵으로 연결
	hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiSurface.GetAddressOf()));
	CHECK(hr);

	// Direct2D 비트맵 프로퍼티 정의 (픽셀 포맷, DPI 등)
	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.dpiX = 96.0f;
	bitmapProperties.dpiY = 96.0f;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.colorContext = NULL;

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

void Graphics::SetFullScreen(bool fullscreen)
{
	HRESULT hr = swapChain->SetFullscreenState(fullscreen, nullptr);
	CHECK(hr);
}

void Graphics::SetResolution(UINT width, UINT height)
{
	if (width == 0 || height == 0) return;
	
	gWinWidth = float(width);
	gWinHeight = float(height);
	Camera::Get()->SetProjection(gWinWidth, gWinHeight);

	RECT rect = { 0,0,(LONG)width,(LONG)height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	UINT left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2 + rect.left;
	UINT top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2 + rect.top;
	MoveWindow(gHandle, left, top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

	deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	d2DeviceContext->SetTarget(nullptr);
	// 1. 기존 리소스 해제
	rtv.Reset();               // Render Target View 해제
	d2Bitmap.Reset();          // Direct2D 비트맵 해제
	dxgiSurface.Reset();       // DXGI 서피스 해제

	// 2. 스왑 체인 크기 조정
	HRESULT hr = swapChain->ResizeBuffers
	(
		0,							// 버퍼 수 (0이면 변경 없음)
		width,						// 새로운 너비
		height,						// 새로운 높이
		DXGI_FORMAT_B8G8R8A8_UNORM, // 픽셀 포맷
		0							// 추가 플래그 (보통 0)
	);
	CHECK(hr);

	// 3. 렌더 타겟 뷰 재생성
	CreateBackBuffer();

	// 4. Direct2D 비트맵 재생성
	hr = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(dxgiSurface.GetAddressOf()));
	CHECK(hr);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = {};
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.dpiX = 96.0f;
	bitmapProperties.dpiY = 96.0f;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	hr = d2DeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &d2Bitmap);
	CHECK(hr);

	d2DeviceContext->SetTarget(d2Bitmap.Get());
}

bool Graphics::LoadCustomFont(const std::wstring& fontFilePath)
{
	// FR_PRIVATE: 현재 프로세스에서만 등록
	int ret = AddFontResourceExW(fontFilePath.c_str(), FR_NOT_ENUM, 0);
	if (ret == 0)
	{
		// 등록 실패
		// 보통은 여기서 로그 출력, 에러 처리
		return false;
	}

	// 필요하다면 전체 윈도우에 폰트가 변경되었음을 알림
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	return true;
}

bool Graphics::UnloadCustomFont(const std::wstring& fontFilePath)
{
	// FR_PRIVATE: 현재 프로세스에서만 등록
	int ret = RemoveFontResourceExW(fontFilePath.c_str(), FR_NOT_ENUM, 0);
	if (ret == 0)
	{
		// 해제 실패
		// 보통은 여기서 로그 출력, 에러 처리
		return false;
	}

	// 필요하다면 전체 윈도우에 폰트가 제거되었음을 알림
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	return true;
}

void Graphics::LoadFont()
{
	LoadCustomFont(FontPath + L"Bazzi OTF.otf");
	LoadCustomFont(FontPath + L"DNFBitBitv2.otf");
	LoadCustomFont(FontPath + L"DNFForgedBlade-Bold.otf");
	LoadCustomFont(FontPath + L"DNFForgedBlade-Light.otf");
	LoadCustomFont(FontPath + L"DNFForgedBlade-Medium.otf");
	LoadCustomFont(FontPath + L"Mabinogi_Classic_OTF.otf");
	LoadCustomFont(FontPath + L"Maplestory OTF Bold.otf");
	LoadCustomFont(FontPath + L"Maplestory OTF Light.otf");
	LoadCustomFont(FontPath + L"NEXON Kart Gothic OTF Bold.otf");
	LoadCustomFont(FontPath + L"NEXON Kart Gothic OTF ExtraBold.otf");
	LoadCustomFont(FontPath + L"NEXON Kart Gothic OTF Medium.otf");
	LoadCustomFont(FontPath + L"NEXON_Warhaven_OTF_Bold.otf");
	LoadCustomFont(FontPath + L"NEXON_Warhaven_OTF_Regular.otf");
	LoadCustomFont(FontPath + L"PretendardJP-Medium.otf");
}

void Graphics::UnloadFont()
{
	UnloadCustomFont(FontPath + L"Bazzi OTF.otf");
	UnloadCustomFont(FontPath + L"DNFBitBitv2.otf");
	UnloadCustomFont(FontPath + L"DNFForgedBlade-Bold.otf");
	UnloadCustomFont(FontPath + L"DNFForgedBlade-Light.otf");
	UnloadCustomFont(FontPath + L"DNFForgedBlade-Medium.otf");
	UnloadCustomFont(FontPath + L"Mabinogi_Classic_OTF.otf");
	UnloadCustomFont(FontPath + L"Maplestory OTF Bold.otf");
	UnloadCustomFont(FontPath + L"Maplestory OTF Light.otf");
	UnloadCustomFont(FontPath + L"NEXON Kart Gothic OTF Bold.otf");
	UnloadCustomFont(FontPath + L"NEXON Kart Gothic OTF ExtraBold.otf");
	UnloadCustomFont(FontPath + L"NEXON Kart Gothic OTF Medium.otf");
	UnloadCustomFont(FontPath + L"NEXON_Warhaven_OTF_Bold.otf");
	UnloadCustomFont(FontPath + L"NEXON_Warhaven_OTF_Regular.otf");
	UnloadCustomFont(FontPath + L"PretendardJP-Medium.otf");
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo() {}

D3DEnumOutputInfo::~D3DEnumOutputInfo() {}

