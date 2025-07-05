#pragma once

class InputLayout
{
private:
	ComPtr<ID3D11InputLayout> inputLayout;

public:
	void Create(const D3D11_INPUT_ELEMENT_DESC* desc, const size_t& count, ID3DBlob* blob)
	{
		HRESULT hr = DEVICE->CreateInputLayout
		(
			desc,
			UINT(count),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&inputLayout
		);
		CHECK(hr);
	}

	void SetIA()
	{
		DC->IASetInputLayout(inputLayout.Get());
	}


public:
	ID3D11InputLayout* GetResource() const { return inputLayout.Get(); }

};

