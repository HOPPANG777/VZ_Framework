#pragma once

// C++의 struct는 기본 접근 제어가 public이라는 차이점이 C의 struct와의 유일한 차이점이다
struct VertexColor
{
	Vector2 position;
	Color color;

	VertexColor() = default;
	VertexColor(const Vector3& position, const Color& color)
		: position(position), color(color)
	{
		
	};

	static vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};



struct VertexTexture
{
	Vector2 position;
	Vector2 uv;

	VertexTexture() = default;
	VertexTexture(const Vector3& position, const Vector3& uv)
		: position(position), uv(uv)
	{

	}

	static vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};



