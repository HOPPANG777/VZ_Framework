#include "Framework.h"
#include "Drawable.h"

Drawable::Drawable(const std::string& name, const Vector3& position, const Vector3& scale, const float rotation, TransformType type, const std::wstring& shaderPath)
	: GameObject(name, position, scale, rotation, type), shaderPath(shaderPath)
{
	//VertexBuffer
	{
		vertexBuffer = std::make_unique<VertexBuffer>();
	}

	//VertexShader
	{
		vertexShader = std::make_unique<VertexShader>();
		vertexShader->Create(shaderPath, "VS");
	}

	//IndexBuffer
	{
		indexBuffer = std::make_unique<IndexBuffer>();
	}

	//InputLayout
	{
		inputLayout = std::make_unique<InputLayout>();
	}

	//PixelShader
	{
		pixelShader = std::make_unique <PixelShader>();
		pixelShader->Create(shaderPath, "PS");
	}
}

void Drawable::Update()
{
	BASE::Update();
}

void Drawable::Render()
{
	BASE::Render();

	vertexBuffer->SetIA();
	indexBuffer->SetIA();
	inputLayout->SetIA();

	vertexShader->SetShader();

	pixelShader->SetShader();;
}

void Drawable::DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, const bool indexed)
{
	Graphics::Get()->GetDC()->IASetPrimitiveTopology(topology);

	if (indexed) Graphics::Get()->GetDC()->DrawIndexed(indexBuffer->GetCount(), 0, 0);
	else Graphics::Get()->GetDC()->Draw(vertexBuffer->GetCount(), 0);
}
