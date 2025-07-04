#pragma once

// C++의 struct는 기본 접근 제어가 public이라는 차이점이 C의 struct와의 유일한 차이점이다

struct VertexColor
{
    Vector3 position;
    Color color;

    VertexColor() = default;
    VertexColor(const Vector3& position, const Color& color)
        : position(position), color(color)
    {

    };

	static std::vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};



struct VertexTexture
{
	Vector3 position;
	Vector2 uv;

	VertexTexture() = default;
	VertexTexture(const Vector3& position, const Vector2& uv)
		: position(position), uv(uv)
	{

	}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
};

struct VertexTile
{
	Vector3 position;
	Vector2 uv;
	Vector2 uv2;
	Color color;
	UINT index;
	
	VertexTile() = default;
	VertexTile(const Vector3& position, const Vector2& uv, const Vector2& uv2, const Color& color, const UINT& index)
		: position(position), uv(uv), uv2(uv2), color(color), index(index)
	{

	}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> isDescs;
};



