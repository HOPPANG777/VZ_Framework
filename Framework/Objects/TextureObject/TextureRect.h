	#pragma once


	class TextureRect : public Drawable
	{
	private:
		vector<VertexTexture> vertices;

		vector<UINT> indices;
	
		ComPtr<ID3D11RasterizerState> rs = nullptr;

		ComPtr<ID3D11BlendState> blend[2];

		ComPtr<ID3D11SamplerState> sampler[2];

		//std::shared_ptr<Texture2D> texture;

		ComPtr<ID3D11ShaderResourceView> srv;
		DirectX::TexMetadata metaData;

	public:
		TextureRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"");
		~TextureRect();

	public:
		void Update() override;
		void Render() override;

	public:


	};

