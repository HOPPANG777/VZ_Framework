#pragma once

/*
DirectX 11
DirectX June 2010 SDK
software development kit
*/
class D3DEnumAdapterInfo;
class D3DEnumOutputInfo;

class Graphics
{
	DECLARE_SINGLETON(Graphics);

public:
	void Init();
	void CreateBackBuffer();

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDC() const { return deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() const { return swapChain.Get(); }
	ID2D1DeviceContext* GetD2DC() const { return d2DeviceContext.Get(); }
	IDWriteFactory* GetWrite() const { return writeFactory.Get(); }

	void Begin();
	void End();

	void GUI();

	void SetFullScreen(bool fullscreen);

	void SetResolution(UINT width, UINT height);

	bool LoadCustomFont(const wstring& fontFilePath);

	bool UnloadCustomFont(const wstring& fontFilePath);

public:
	void LoadFont();
	void UnloadFont();

private:
	void EnumerateAdapters();
	bool EnumerateAdapterOutput(D3DEnumAdapterInfo* adapterInfo);

private:
	void CreateDirect3D();
	void CreateDirect2D();
	void CreateDirectWrite();

private:
	ComPtr<ID3D11Device> device = nullptr;
	// 그래픽 리소스 생성, GPU기능 정의하는 영역

	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// 모든 렌더링 명령, 파이프라인 상태 설정, 리소스 바인딩, 그리기, 데이터 전송 등 GPU에 명령을 내리는 영역

	ComPtr<IDXGISwapChain> swapChain = nullptr;
	// 프레임 버퍼를 관리하는 스왑 체인, 화면에 표시할 백 버퍼와 프론트 버퍼를 관리

	ComPtr<ID2D1Device> d2Device = nullptr;
	ComPtr<ID2D1DeviceContext> d2DeviceContext = nullptr;

	ComPtr<ID2D1Factory1> d2Factory = nullptr;
	ComPtr<IDWriteFactory> writeFactory = nullptr;

	ComPtr<ID2D1Bitmap1> d2Bitmap = nullptr;

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGISurface> dxgiSurface = nullptr;

	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	D3D11_VIEWPORT viewport = { 0 };
	Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };

	UINT numerator = 0;
	UINT denominator = 1;

	UINT gpuMemorySize;
	wstring gpuName;

	vector<unique_ptr<D3DEnumAdapterInfo>> adapterInfos;
	int selectedAdapterIndex = 0;

	bool bVsync = true;
};

//Color clearColor = Color(0xcff0cc);

class D3DEnumAdapterInfo
{
public:
	~D3DEnumAdapterInfo();

	UINT adapterOrdinal = 0;
	ComPtr<IDXGIAdapter1> adapter = nullptr;
	DXGI_ADAPTER_DESC1 adapterDesc = { 0 };

	shared_ptr<D3DEnumOutputInfo> outputInfo = nullptr;
};

class D3DEnumOutputInfo
{
public:
	~D3DEnumOutputInfo();

	ComPtr<IDXGIOutput> output = nullptr;
	DXGI_OUTPUT_DESC outputDesc = { 0 };

	UINT numerator = 0;
	UINT denominator = 1;
};