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

	TransformType type;

public:
	Drawable(const string& name, const Vector3& position, const Vector3& scale, const float rotation, TransformType type, const wstring& shaderPath = L"");
	
	Drawable(const Drawable& other)
		: Drawable(other.name, other.GetPosition(), other.GetScale(), other.GetRotation(), other.type, other.shaderPath)
	{

	}

	Drawable operator=(const Drawable& other)
	{
		if (this != &other)
		{
			Object::operator=(other); // 상위 객체 복사
			type = other.type;
			shaderPath = other.shaderPath;
			// 기타 리소스 복사
		}
		return *this;
	}

	virtual ~Drawable() override = default;
public:
	void Update() override;
	void Render() override;

	void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, const bool indexed = true);
};

