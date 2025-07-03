#pragma once
#include "Framework.h"
#include "Systems/Graphics.h"

class InputLayout
{
private:
	ComPtr<ID3D11InputLayout> inputLayout;

public:
	void Create(const D3D11_INPUT_ELEMENT_DESC* desc, const size_t& count, ID3DBlob* blob)
	{
		HRESULT hr = Graphics::Get()->GetDevice()->CreateInputLayout
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
		Graphics::Get()->GetDC()->IASetInputLayout(inputLayout.Get());
	}


public:
	ID3D11InputLayout* GetResource() const { return inputLayout.Get(); }

};

