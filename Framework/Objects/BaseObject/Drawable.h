#pragma once

#include "GameObject.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"

class Drawable : public GameObject
{
protected:
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;

	std::unique_ptr<VertexShader> vertexShader;

	std::unique_ptr<InputLayout> inputLayout;

	std::unique_ptr<PixelShader> pixelShader;

	std::wstring shaderPath = L"";

	TransformType type;

public:
	Drawable(const std::string& name, const Vector3& position, const Vector3& scale, const float rotation, TransformType type, const std::wstring& shaderPath = L"");
	
	Drawable(const Drawable& other)
		: Drawable(other.name, other.GetPosition(), other.GetScale(), other.GetRotation(), other.type, other.shaderPath)
	{

	}

	Drawable operator=(const Drawable& other)
	{
		if (this != &other)
		{
			GameObject::operator=(other); // 상위 객체 복사
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

