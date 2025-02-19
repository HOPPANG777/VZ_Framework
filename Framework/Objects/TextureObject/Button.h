#pragma once

class Button : public Drawable
{
private:
	vector<VertexTexture> vertices;

	vector<UINT> indices;

	ComPtr<ID3D11RasterizerState> rs = nullptr;

	ComPtr<ID3D11BlendState> blend[2];

	ComPtr<ID3D11SamplerState> sampler[2];

	ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// 컬러 변환을 위한 상수 버퍼 (ColorBuffer)
	unique_ptr<ColorBuffer> colorBuffer;

private:
	// 클릭 이벤트를 처리할 콜백 함수
	function<void()> onClick = nullptr;

	// 클릭 여부를 관리할 간단한 상태 변수
	bool wasPressed = false; // 이전 프레임에서 마우스를 누르고 있었는지

private:
	bool IsMouseOver(); // 내부에서 마우스가 버튼 위에 있는지 판단

public:
	// 생성자: shaderPath를 통해 버튼에 사용할 텍스처/머티리얼 정보를 설정
	Button(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"", const wstring shaderPath = ShaderPath + L"VertexTexture.hlsl");
	~Button();

public:
	void Update();
	void Render();
	
public:
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	void SetImage(wstring imagePath)
	{
		// SRV 추출 // 그림 데이터 추출
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			imagePath.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		CHECK(hr);
	}

public:
	Color GetColor() const { return colorBuffer->GetColor();; }

	void SetColor(const Color& color)
	{
		colorBuffer->SetColor(color);
		// 별도로 정점 버퍼의 데이터를 변경할 필요는 없습니다.
		// 픽셀 셰이더에서는 상수 버퍼의 색상 값을 사용하여 텍스처 색상을 변경합니다.
	}

public:
	// 외부에서 콜백을 등록할 수 있도록 SetOnClick 함수를 제공
	void SetOnClick(const function<void()>& callback) { onClick = callback; }

};