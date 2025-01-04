#pragma once

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

		DC->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	void SetPSBuffer(const UINT slot)
	{
		MapData(data, dataSize);

		DC->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

protected:
	ConstantBuffer(void* data, const UINT dataSize)
		: data(data), dataSize(dataSize)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = dataSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

private:
	void MapData(void* data, const UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		DC->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		{
			memcpy(mappedSubResource.pData, data, dataSize);
		}
		DC->Unmap(buffer.Get(), 0);
	}

};

