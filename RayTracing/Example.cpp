
// 레이트레이싱 예제 구현 파일
#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Example.h"

Example::Example(HWND window, UINT width, UINT height)
	: rayTracer(width/8, height/8) //width / 2.0f, height / 2.0f 이걸로 해상도 낮출수 있음
{
	Initialize(window, width, height);
}

void Example::InitShaders()
{
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;

#ifdef _DEBUG
	ComPtr<ID3DBlob> errorBlob;

	if (FAILED(D3DCompileFromFile(L"_Shaders/VertexShader.hlsl", 0, 0, "main", "vs_5_0",
		0, 0, &vsBlob, &errorBlob)))
	{
		if (errorBlob.Get())
		{
			const std::string& str = (const char*)errorBlob->GetBufferPointer();
			MessageBoxA(nullptr, str.c_str(), "Vertex Shader Compile Error!", MB_OK);
		}
		assert(false);
	}

	if (FAILED(D3DCompileFromFile(L"_Shaders/PixelShader.hlsl", 0, 0, "main", "ps_5_0",
		0, 0, &psBlob, &errorBlob)))
	{
		if (errorBlob.Get())
		{
			const std::string& str = (const char*)errorBlob->GetBufferPointer();
			MessageBoxA(nullptr, str.c_str(), "Pixel Shader Compile Error!", MB_OK);
		}
		assert(false);
	}

#elif NDEBUG
	DX::ReadData(L"_Shaders/VertexShader.cso", &vsBlob);

	DX::ReadData(L"_Shaders/PixelShader.cso", &psBlob);

#endif

	CHECK(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
	CHECK(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader));

	vector<D3D11_INPUT_ELEMENT_DESC> ieDescs =
	{
	   { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CHECK(device->CreateInputLayout(ieDescs.data(), UINT(ieDescs.size()), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout));
	deviceContext->IASetInputLayout(inputLayout.Get());
}

void Example::Initialize(HWND window, UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;

	const vector<D3D_FEATURE_LEVEL> featureLevelArray = { D3D_FEATURE_LEVEL_11_0 };

	CHECK(D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevelArray.data(),
		UINT(featureLevelArray.size()),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext));

	// CreateRenderTarget
	ComPtr<ID3D11Texture2D> backBuffer;
	CHECK(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	CHECK(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));
	backBuffer.Reset();

	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = float(width);
	viewport.Height = float(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);

	InitShaders();

	// CreateSamplerState
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CHECK(device->CreateSamplerState(&samplerDesc, &colorSampler));

	// Create Texture and RenderTarget
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = rayTracer.width;
	textureDesc.Height = rayTracer.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	CHECK(device->CreateTexture2D(&textureDesc, nullptr, &canvasTexture));

	CHECK(device->CreateShaderResourceView(canvasTexture.Get(), nullptr, &canvasTextureView));

	// Create VertexBuffer
	{
		const vector<Vertex> vertices =
		{
		   { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		   { { 1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		   { { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		   { { -1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		};

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.ByteWidth = UINT(sizeof(vertices[0]) * vertices.size());
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.StructureByteStride = sizeof(vertices[0]);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = vertices.data();

		CHECK(device->CreateBuffer(&bufferDesc, &vertexBufferData, &vertexBuffer));
	}

	//CreateIndexBuffer
	{
		const vector<uint16_t> indices =
		{
		   3, 1, 0,
		   2, 1, 3
		};

		indexCount = UINT(indices.size());

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = UINT(sizeof(indices[0]) * indices.size());
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.StructureByteStride = sizeof(indices[0]);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();

		CHECK(device->CreateBuffer(&bufferDesc, &indexBufferData, &indexBuffer));
	}
}

void Example::Update()
{
	vector<Color> pixels(rayTracer.width * rayTracer.height, backgroundColor);

	rayTracer.Render(pixels);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	deviceContext->Map(canvasTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	{
		memcpy(mappedSubresource.pData, pixels.data(), sizeof(pixels[0]) * pixels.size());
	}
	deviceContext->Unmap(canvasTexture.Get(), 0);
}

void Example::Render()
{
	

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);

	//IA
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//VS
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);

	//PS
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	deviceContext->PSSetShaderResources(0, 1, canvasTextureView.GetAddressOf());
	deviceContext->PSSetSamplers(0, 1, colorSampler.GetAddressOf());

	//DrawCall(DPCall)
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
