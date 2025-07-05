#pragma once


class ColorCircle : public Drawable, public IColorable
{
private:
	vector<VertexColor> vertices;

	//vector<UINT> indices;

	size_t segments = 0;

	Color color;

public:
	ColorCircle(const Vector3& position, const Vector3& scale, const float rotation, const size_t& segments, const Color& color = RED);
	~ColorCircle();

public:
	void Update() override;
	void Render() override;

public:
	virtual Color GetColor() const override { return color; }
	virtual void SetColor(const Color& color) override
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

