#pragma once
#include "IShader.h"

class PixelShader : public IShader
{
private:
	ComPtr<ID3D11PixelShader> shader;

public:
	void Create(const std::wstring& path, const std::string& entryName) override
	{
		this->path = path;
		this->entryName = entryName;

		CompileShader(path, entryName, "ps_5_0", blob.GetAddressOf());
		HRESULT hr = Graphics::Get()->GetDevice()->CreatePixelShader
		(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			&shader
		);
		CHECK(hr);
	}

	void Clear() override
	{
		shader.Reset();
		blob.Reset();
	}

	void SetShader() override
	{
		Graphics::Get()->GetDC()->PSSetShader(shader.Get(), nullptr, 0);
	}

public:
	ID3D11PixelShader* GetResource() const { return shader.Get(); }

};

