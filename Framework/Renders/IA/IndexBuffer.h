#pragma once
#include "Framework.h"
#include "Systems/Graphics.h"

class IndexBuffer
{
private:
	ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;

public:
	void Create(const std::vector<UINT>& indices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT)
	{
		stride = sizeof(UINT);
		count = UINT(indices.size());

		D3D11_BUFFER_DESC desc = { 0 };

		desc.Usage = usage;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
		subData.pSysMem = indices.data();

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, &subData, &buffer);
		CHECK(hr);
	};

	void SetIA()
	{
		Graphics::Get()->GetDC()->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	};

public:
	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }
};


