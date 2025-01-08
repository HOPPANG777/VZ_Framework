	#pragma once


	class TextureRect : public Drawable
	{
	protected:
		vector<VertexTexture> vertices;

		vector<UINT> indices;
	
		ComPtr<ID3D11RasterizerState> rs = nullptr;

		ComPtr<ID3D11BlendState> blend[2];

		ComPtr<ID3D11SamplerState> sampler[2];

		ComPtr<ID3D11ShaderResourceView> srv = nullptr;
		
		D3D11_MAPPED_SUBRESOURCE subRe;

		//std::shared_ptr<Texture2D> texture;

	public:
		TextureRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"");
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

	};

