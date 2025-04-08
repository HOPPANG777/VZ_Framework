#include "Framework.h"
#include "ColorFilledCircle.h"

ColorFilledCircle::ColorFilledCircle(const Vector3& position, const Vector3& scale, const float rotation, const size_t& segments, const Color& color)
	: Drawable("ColorFilledCircle", position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexColor.hlsl"), segments(segments), color(color)
{
	//vertices
	{
		vertices.assign(segments + 1, VertexColor());
		vertices[0].position = Vector2();

		for (size_t i = 1; i <= segments; ++i)
		{
			float theta = 2 * Values::PI * i / segments;

			vertices[i].position = Vector2(sinf(theta), cosf(theta)) * 0.5f;
		}

		for (VertexColor& vertex : vertices) vertex.color = color;
	}

	//vertexBuffer
	{
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		indices.assign(segments * 3, 0);

		for (size_t i = 0; i < segments; ++i)
		{
			indices[i * 3] = 0;
			indices[i * 3 + 1] = UINT(i) + 1;
			indices[i * 3 + 2] = UINT(i) + 2;
		}

		indices.back() = 1;
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

ColorFilledCircle::~ColorFilledCircle()
{
}

void ColorFilledCircle::Update()
{
	BASE::Update();
}

void ColorFilledCircle::Render()
{
	BASE::Render();

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
