#include "Framework.h"
#include "Drawable.h"

Drawable::Drawable(const string& name, const Vector3& position, const Vector3& scale, const float rotation, TransformType type, const wstring& shaderPath)
	: Object(name, position, scale, rotation, type), shaderPath(shaderPath)
{
	//VertexBuffer
	{
		vertexBuffer = make_unique<VertexBuffer>();
	}

	//VertexShader
	{
		vertexShader = make_unique<VertexShader>();
		vertexShader->Create(shaderPath, "VS");
	}

	//IndexBuffer
	{
		indexBuffer = make_unique<IndexBuffer>();
	}

	//InputLayout
	{
		inputLayout = make_unique<InputLayout>();
	}

	//PixelShader
	{
		pixelShader = make_unique <PixelShader>();
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
	DC->IASetPrimitiveTopology(topology);

	if (indexed) DC->DrawIndexed(indexBuffer->GetCount(), 0, 0);
	else DC->Draw(vertexBuffer->GetCount(), 0);
}
