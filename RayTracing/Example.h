// 레이트레이싱 예제 헤더 파일
#pragma once

#include "RayTracer.h"

struct Vertex
{
	Vector4 pos;
	Vector2 uv;
};

class Example
{
public:
	Example(HWND window, UINT width, UINT height);

	void InitShaders();

	void Initialize(HWND window, UINT width, UINT height);

	Vector2 TransformScreenToWorld(const Vector2& screenPos);

public:
	void Update();
	void Render();

public:
	IDXGISwapChain* GetSwapChain() const { return swapChain.Get(); }
	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetSDeviceContext() const { return deviceContext.Get(); }

private:
	// 유니크 포인터 같은 애
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	D3D11_VIEWPORT viewport = { 0 };
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	//자원 : 버퍼, 텍스쳐
	ComPtr <ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Texture2D> canvasTexture;
	ComPtr<ID3D11ShaderResourceView> canvasTextureView;
	ComPtr<ID3D11SamplerState> colorSampler;

	UINT indexCount = 0;

	UINT width = 0, height = 0;
	Color backgroundColor = { .5f, .5f, .5f, 1.0f };
	//Color backgroundColor = Color(Vector3(127, 161, 195) / 250.f);

	THC::RayTracer rayTracer;
};