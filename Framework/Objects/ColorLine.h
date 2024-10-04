#pragma once

class ColorLine : public Drawable
{
private:
	vector<VertexColor> vertices;

	vector<UINT> indices;

	Color color;

public:
	ColorLine(const Vector2& position, const Vector2& scale, const float rotation, const Color& color = RED);
	~ColorLine();

	void Update();
	void Render();

public:
	Color GetColor() const { return color; }
	void SetColor(const Color& color)
	{
		this->color = color;
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		{ // 맵핑중인 공간

			// vertices에다가 변경된 color값을 넣어주고
			for (VertexColor& vertex : vertices) vertex.color = color;

			// 변경된 color값을 vertices에다가 넣어준다.
			memcpy(mappedSubResource.pData, vertices.data(), vertexBuffer->GetCount() * vertexBuffer->GetStride());
		}
		DC->Unmap(vertexBuffer->GetResource(), 0);
	}
};

