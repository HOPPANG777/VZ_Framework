#include "Framework.h"
#include "ColorLine.h"

ColorLine::ColorLine(const Vector3& position, const float& length, const float rotation, const Color& color)
	: Drawable("ColorLine", position, Vector3(length, 0, 0), rotation, TransformType::Normal, ShaderPath + L"VertexColor.hlsl"), color(color)
{
	//vertices
	{
        vertices.assign(2, VertexColor());

        // 시작 지점
        vertices[0].position = Vector3(0, 0, 0);
        // 끝 지점 (길이만큼 X축으로 이동)
        vertices[1].position = Vector3(1, 0, 0);

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

ColorLine::~ColorLine()
{
}

void ColorLine::Update()
{
	BASE::Update();
}

void ColorLine::Render()
{
	BASE::Render();

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, false);
}
