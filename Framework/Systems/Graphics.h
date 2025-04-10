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

private:
	void EnumerateAdapters();
	bool EnumerateAdapterOutput(D3DEnumAdapterInfo* adapterInfo);

private:
	void CreateDirect3D();
	void CreateDirect2D();
	void CreateDirectWrite();

private:
	ComPtr<ID3D11Device> device = nullptr;
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	ComPtr<IDXGISwapChain> swapChain = nullptr;

	ComPtr<ID2D1Device>            d2Device = nullptr;
	ComPtr<ID2D1DeviceContext>     d2DeviceContext = nullptr;

	ComPtr<ID2D1Factory1>          d2Factory = nullptr;
	ComPtr<IDWriteFactory>         writeFactory = nullptr;

	ComPtr<ID2D1Bitmap1>           d2Bitmap = nullptr;

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGISurface> dxgiSurface = nullptr;

	ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	D3D11_VIEWPORT viewport = { 0 };
	Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };
	//Color clearColor = Color(0xcff0cc);

	UINT numerator = 0;
	UINT denominator = 1;

	UINT gpuMemorySize;
	wstring gpuName;

	vector<unique_ptr<D3DEnumAdapterInfo>> adapterInfos;
	int selectedAdapterIndex = 0;

	bool bVsync = true;
};

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