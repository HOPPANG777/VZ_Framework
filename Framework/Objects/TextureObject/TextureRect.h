	#pragma once

	class TextureRect : public Drawable, public IColorable
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

	public:
		TextureRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"", const TransformType type = TransformType::Normal, const wstring shaderPath = ShaderPath + L"VertexTexture.hlsl");
		~TextureRect();

	public:
		void Update() override;
		void Render() override;

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
		virtual Color GetColor() const override { return colorBuffer->GetColor();; }

		virtual void SetColor(const Color& color) override
		{
			colorBuffer->SetColor(color);
			// 별도로 정점 버퍼의 데이터를 변경할 필요는 없습니다.
			// 픽셀 셰이더에서는 상수 버퍼의 색상 값을 사용하여 텍스처 색상을 변경합니다.
		}
	};

