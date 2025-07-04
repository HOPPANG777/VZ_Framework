#pragma once
#include "Framework.h"
#include "Systems/Graphics.h"

class ConstantBuffer
{
private:
	D3D11_BUFFER_DESC desc = { 0 };
	ComPtr<ID3D11Buffer> buffer;

	void* data;
	UINT dataSize;

public:
	void SetVSBuffer(const UINT slot)
	{
		MapData(data, dataSize);

		Graphics::Get()->GetDC()->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	void SetPSBuffer(const UINT slot)
	{
		MapData(data, dataSize);

		Graphics::Get()->GetDC()->CSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

protected:
	ConstantBuffer(void* data, const UINT dataSize)
		: data(data), dataSize(dataSize)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = dataSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

private:
	void MapData(void* data, const UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		Graphics::Get()->GetDC()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		{
			memcpy(mappedSubResource.pData, data, dataSize);
		}
		Graphics::Get()->GetDC()->Unmap(buffer.Get(), 0);
	}

};

