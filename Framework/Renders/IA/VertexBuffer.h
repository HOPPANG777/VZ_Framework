#pragma once
#include "Framework.h"
#include "Systems/Graphics.h"

class VertexBuffer
{
private:
	ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;

public:
	template <typename T>
	void Create(const std::vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

	void SetIA()
	{
		Graphics::Get()->GetDC()->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
	};

public:
	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE& usage)
{
	stride = sizeof(T);
	count = UINT(vertices.size());

	D3D11_BUFFER_DESC desc = { 0 };

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = stride * count;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = vertices.data();

	HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}