#include "Framework.h"
#include "ColorCircle.h"

ColorCircle::ColorCircle(const Vector2& position, const Vector2& scale, const float rotation, const Color& color)
	: Drawable("ColorLine", position, scale, rotation, L"../_Shaders/VertexColor.hlsl"), color(color)
{
	//vertices
	{
		vertices.assign(4, VertexColor());

		// 좌 하단
		vertices[0].position = { -0.5f, -0.5f };
		// 좌 상단
		vertices[1].position = { -0.5f, 0.5f };
		// 우 하단
		vertices[2].position = { 0.5f, -0.5f };
		// 우 상단
		vertices[3].position = { 0.5f, 0.5f };

		for (auto& vertex : vertices) vertex.color = color;
	}

	//vertexBuffer
	{
		//객체를 만들어 주어야 한다.
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//indexBuffer
	{
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//inputLayout
	{
		inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
	}
}

ColorCircle::~ColorCircle()
{

}

void ColorCircle::Update()
{
	BASE::Update();
}

void ColorCircle::Render()
{
	BASE::Render();

	DrawCall(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
