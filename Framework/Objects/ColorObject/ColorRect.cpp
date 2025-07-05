#include "Framework.h"
#include "ColorRect.h"

ColorRect::ColorRect(const Vector3& position, const Vector3& scale, const float rotation, const Color& color)
	: Drawable("ColorRect", position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexColor.hlsl"), color(color)
{
	//vertices
	{
		vertices.assign(4, VertexColor());

		// 좌 하단
		vertices[0].position = { -0.5f, -0.5f, 0 };
		// 좌 상단
		vertices[1].position = { -0.5f, 0.5f, 0 };
		// 우 하단
		vertices[2].position = { 0.5f, -0.5f, 0 };
		// 우 상단
		vertices[3].position = { 0.5f, 0.5f, 0 };
	
		for (VertexColor& vertex : vertices) vertex.color = color;
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

ColorRect::~ColorRect()
{

}

void ColorRect::Update()
{
	BASE::Update();
}

void ColorRect::Render()
{
	BASE::Render();

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


#pragma region
////vertices
//{
//	vertices.assign(4, VertexColor());
//
//	// 좌 하단
//	vertices[0].position = { -0.5f, -0.5f };
//	// 좌 상단
//	vertices[1].position = { -0.5f, 0.5f };
//	// 우 하단
//	vertices[2].position = { 0.5f, -0.5f };
//	// 우 상단
//	vertices[3].position = { 0.5f, 0.5f };
//
//	for (auto& vertex : vertices) vertex.color = color;
//}
//
////vertexBuffer
//{
//	//객체를 만들어 주어야 한다.
//	//vertexBuffer = make_unique<VertexBuffer>();
//	vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
//}
//
////indices
//{
//	indices = { 0, 1, 2, 2, 1, 3 };
//}
//
////indexBuffer
//{
//	//indexBuffer = make_unique<IndexBuffer>();
//	indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
//}
//
////vertexShader
//{
//	//vertexShader = make_unique <VertexShader>();
//	//vertexShader->Create(L"../_Shaders/VertexColor.hlsl", "VS");
//}
//
////inputLayout
//{
//	//inputLayout = make_unique<InputLayout>();
//	//inputLayout->Create(VertexColor::ieDescs.data(), VertexColor::ieDescs.size(), vertexShader->GetBlob());
//}
//
////pixelShader
//{
//	//pixelShader = make_unique<PixelShader>();
//	//pixelShader->Create(L"../_Shaders/VertexColor.hlsl", "PS");
//}
//
////SRT
//{
//	//S = XMMatrixScalingFromVector(scale);
//	//R = XMMatrixRotationZ(-this->rotation);
//	//T = XMMatrixTranslationFromVector(position);
//}
//
////world
//{
//	//world = S * R * T;
//}
//
////worldBuffer
//{
//	//worldBuffer = make_unique<WorldBuffer>();
//	//worldBuffer->SetWorld(world);
//}
#pragma endregion

