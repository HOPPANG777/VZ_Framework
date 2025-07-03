#pragma once
#include "IShader.h"

class VertexShader : public IShader
{
private:
	ComPtr<ID3D11VertexShader> shader;

public:
	void Create(const wstring& path, const string& entryName) override
	{
		this->path = path;
		this->entryName = entryName;

		CompileShader(path, entryName, "vs_5_0", blob.GetAddressOf());
		HRESULT hr = Graphics::Get()->GetDevice()->CreateVertexShader
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
		Graphics::Get()->GetDC()->VSSetShader(shader.Get(), nullptr, 0);
	}

public:
	ID3D11VertexShader* GetResource() const { return shader.Get(); }


};

