#include "Framework.h"
#include "ColorCircle.h"

ColorCircle::ColorCircle(const Vector3& position, const Vector3& scale, const float rotation, const size_t& segments, const Color& color)
	: Drawable("ColorCircle", position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexColor.hlsl"), segments(segments), color(color)
{
	//vertices
	{
		vertices.assign(segments + 1, VertexColor());

		for (size_t i = 0; i <= segments; ++i)
		{
			float theta = 2 * Values::PI * i / segments;

			vertices[i].position = Vector2(sinf(theta), cosf(theta)) * 0.5f;
		}

		for (VertexColor& vertex : vertices) vertex.color = color;
	}

	//vertexBuffer
	{
		//객체를 만들어 주어야 한다.
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		//indices = { 0, 1, 2, 2, 1, 3 };
	}

	//indexBuffer
	{
		//indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
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

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, false);
}
