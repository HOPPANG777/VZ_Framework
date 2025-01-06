#pragma once

class Drawable : public Object
{
protected:
	unique_ptr<VertexBuffer> vertexBuffer;
	unique_ptr<IndexBuffer> indexBuffer;

	unique_ptr<VertexShader> vertexShader;

	unique_ptr<InputLayout> inputLayout;

	unique_ptr<PixelShader> pixelShader;

	wstring shaderPath = L"";

public:
	Drawable(const string& name, const Vector3& position, const Vector3& scale, const float rotation, const wstring& shaderPath = L"");
	
	Drawable(const Drawable& other)
		: Drawable(other.name, other.position, other.scale, other.rotation, other.shaderPath)
	{

	}

	Drawable operator=(const Drawable& other)
	{
		Drawable obj(other);
		return obj;
	}

	virtual ~Drawable() override = default;
public:
	void Update() override;
	void Render() override;

	void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, const bool indexed = true);
};

