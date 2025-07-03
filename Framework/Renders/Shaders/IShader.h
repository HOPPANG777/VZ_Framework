#pragma once
#include "Framework.h"
#include "Systems/Graphics.h"

class IShader
{
protected:
	wstring path = L"";
	string entryName = "";
	ComPtr<ID3DBlob> blob;

public:
	virtual ~IShader() = default;

	virtual void Create(const wstring& path, const string& entryName) = 0;

	virtual void Clear() = 0;
	virtual void SetShader() = 0;

public:
	ID3DBlob* GetBlob() const { return blob.Get(); }

protected:
	void CompileShader(const wstring& path, const string& entryName, const string& profile, ID3DBlob** blob)
	{
		ComPtr<ID3DBlob> error;
		HRESULT hr = D3DCompileFromFile
		(
			path.c_str(),
			nullptr,
			nullptr,
			entryName.c_str(),
			profile.c_str(),
			0,
			0,
			blob,
			&error
		);
		CHECK(hr);
	}
	
private:
	void CheckShaderError(const HRESULT& hr, ID3DBlob* error)
	{
		if (FAILED(hr))
		{
			if (error)
			{
				const string& str = (const char*)error->GetBufferPointer();
				MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}
	}

};

