#pragma once

/*
DirectX 11 전용 구현체입니다.
다른 그래픽 API를 사용하려면 IRenderer 인터페이스를 상속받아 구현합니다.
*/
#include "Interfaces/IRenderer.h"
class D3DEnumAdapterInfo;
class D3DEnumOutputInfo;

class Graphics : public IRenderer
{
        DECLARE_SINGLETON(Graphics);

public:
        Graphics();                                    // 생성자
        ~Graphics() override;                          // 소멸자
        void Init() override;                            // 렌더러 초기화
        void CreateBackBuffer();                        // 백 버퍼 생성

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDC() const { return deviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() const { return swapChain.Get(); }
	ID2D1DeviceContext* GetD2DC() const { return d2DeviceContext.Get(); }
	IDWriteFactory* GetWrite() const { return writeFactory.Get(); }

        void Begin() override;                          // 프레임 시작
        void End() override;                            // 프레임 종료

	void GUI();

        void SetFullScreen(bool fullscreen) override;   // 전체 화면 설정

        void SetResolution(UINT width, UINT height) override; // 해상도 변경

	bool LoadCustomFont(const std::wstring& fontFilePath);

	bool UnloadCustomFont(const std::wstring& fontFilePath);

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
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	ComPtr<IDXGISwapChain> swapChain = nullptr;

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
	std::wstring gpuName;

	std::vector<std::unique_ptr<D3DEnumAdapterInfo>> adapterInfos;
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

	std::shared_ptr<D3DEnumOutputInfo> outputInfo = nullptr;
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