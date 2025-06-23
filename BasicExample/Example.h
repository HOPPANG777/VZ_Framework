#pragma once

struct Vertex
{
	Vector4 pos;
	Vector2 uv;
};

class Image
{
private:
	int width = 0, height = 0, channels = 0;
	vector<Color> pixels;

public:
	void ReadFromFile(const char* fileName);
	void WritePNG(const char* fileName);
	Color& GetPixel(int x, int y);

public:
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	vector<Color>& GetPixels() { return pixels; }

public:
	void BoxBlur5();
	void GaussianBlur5();
	void Bloom(float th, int numRepeat, float weight);
};

class Example
{
public:
	Example(HWND window, UINT width, UINT height);

	void InitShaders();

	void Initialze(HWND window, UINT width, UINT height, UINT canvasWidth, UINT canvasHeight);

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

	UINT canvasWidth = 0, canvasHeight = 0;
	Color backgroundColor = { .5f, .5f, .5f, 1.0f };
	//Color backgroundColor = Color(Vector3(127, 161, 195) / 250.f);

	Image image;
};