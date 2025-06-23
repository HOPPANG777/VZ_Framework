#include "pch.h"
#include "Example.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void Image::ReadFromFile(const char* fileName)
{
	unsigned char* img = stbi_load(fileName, &width, &height, &channels, 0);

	if (width)
		cout << width << " " << height << " " << channels << '\n';
	else
		cout << "Error : reading" << fileName << "failed." << '\n';

	pixels.resize(width * height);
	for (int i = 0; i < width * height; ++i)
	{
		pixels[i].x = img[i * channels] / 225.0f;
		pixels[i].y = img[i * channels + 1] / 225.0f;
		pixels[i].z = img[i * channels + 2] / 225.0f;
		pixels[i].w = 1.0f;
	}

	SAFE_DELETE_ARRAY(img);
}

void Image::WritePNG(const char* fileName)
{
	vector<unsigned char> img(width * height * channels, 0);

	for (size_t i = 0; i < width * height; ++i)
	{
		img[i * channels] = uint8_t(pixels[i].x * 225.0f);
		img[i * channels + 1] = uint8_t(pixels[i].y * 225.0f);
		img[i * channels + 2] = uint8_t(pixels[i].z * 225.0f);
	}

	stbi_write_png(fileName, width, height, channels, img.data(), width * channels);
}

Color& Image::GetPixel(int x, int y)
{
	x = std::clamp(x, 0, width - 1);
	y = std::clamp(y, 0, height - 1);

	return pixels[x + width * y];
}

void Image::BoxBlur5()
{
	vector<Color> pixelsBuffer(pixels.size());
	int blurSize = 2;  // 5x5 블러

#pragma omp parallel for
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Color blurColor = Color(0, 0, 0, 1.0f);

			for (int x = -blurSize; x <= blurSize; ++x)
			{
				blurColor += GetPixel(i + x, j);
			}

			float divisor = (float)(2 * blurSize + 1);
			blurColor *= 1.0f / divisor;
			pixelsBuffer[j * width + i] = blurColor;
		}
	}

	swap(pixels, pixelsBuffer);

	// 세로 블러
#pragma omp parallel for
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Color blurColor = Color(0, 0, 0, 1.0f);

			for (int y = -blurSize; y <= blurSize; ++y)
			{
				blurColor += GetPixel(i, j + y);
			}

			float divisor = (float)(2 * blurSize + 1);
			blurColor *= 1.0f / divisor;
			pixelsBuffer[j * width + i] = blurColor;
		}
	}

	swap(pixels, pixelsBuffer);
}

void Image::GaussianBlur5()
{
	vector<Color> pixelsBuffer(pixels.size());

	float gaussianKernel[5] = { 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f };
	int blurSize = 2; // 5x5 블러

	// 가로 블러
#pragma omp parallel for
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Color blurColor = Color(0, 0, 0, 1.0f);

			for (int x = -blurSize; x <= blurSize; ++x)
			{
				blurColor += GetPixel(i + x, j) * gaussianKernel[x + blurSize];
			}

			pixelsBuffer[j * width + i] = blurColor;
		}
	}
	swap(pixels, pixelsBuffer);

	// 세로 블러
#pragma omp parallel for
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Color blurColor = Color(0, 0, 0, 1.0f);

			for (int y = -blurSize; y <= blurSize; ++y)
			{
				blurColor += GetPixel(i, j + y) * gaussianKernel[y + blurSize];
			}

			pixelsBuffer[j * width + i] = blurColor;
		}
	}

	swap(pixels, pixelsBuffer);
}

void Image::Bloom(float th, int numRepeat, float weight)
{
	//1. 원본 이미지를 백업.
	vector<Color> pixelsBackup(pixels.size());
	pixelsBackup = pixels;

	//2. 밝은 픽셀은 그대로 두고 어두분 픽셀은 완전히 검게 만듦
	//	(밝다는 기준은 우리가 정해주면 된다.)
#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Color& pixel = pixels[x + width * y];
			float Luminance = pixel.x * 0.2126 + pixel.y * 0.7152 + pixel.z * 0.0722;

			if (Luminance < th)
			{
				pixel = Color(0, 0, 0, 0);
			}
		}
	}

	//3. 그대로 둔 픽셀에만 블러를 적용하고
	for (int count = 0; count < numRepeat; count++)
	{
		GaussianBlur5();
	}

	//4. 백업 이미지와 밝은 부분만 가우시안블러를 적용한 이미지를 더한다.  
	//5. 가우시안 블러 적용한 이미지에 가중치를 곱해서 강도 조절
	for (size_t i = 0; i < pixelsBackup.size(); i++)
	{
		pixelsBackup[i].x = std::clamp(pixels[i].x * weight + pixelsBackup[i].x, 0.0f, 1.0f);
		pixelsBackup[i].y = std::clamp(pixels[i].y * weight + pixelsBackup[i].y, 0.0f, 1.0f);
		pixelsBackup[i].z = std::clamp(pixels[i].z * weight + pixelsBackup[i].z, 0.0f, 1.0f);
	}

	swap(pixels, pixelsBackup);
}

Example::Example(HWND window, UINT width, UINT height)
{
	image.ReadFromFile("_Textures/cityImage.jpg");

	auto& pixels = image.GetPixels();

	/*for (int i = 0; i < image.GetWidth() * image.GetHeight(); ++i)
	{
		pixels[i].x += 1e-3f;
		pixels[i].y += 1e-3f;
		pixels[i].z += 1e-3f;

		pixels[i].x = clamp(pixels[i].x * 30000.5f, .0f, 1.f);
		pixels[i].y = clamp(pixels[i].y * 30000.5f, .0f, 1.f);
		pixels[i].z = clamp(pixels[i].z * 30000.5f, .0f, 1.f);
	}*/

	//for (int i = 0; i < 100; ++i)
	//	image.BoxBlur5();

	//for (int i = 0; i < 100; ++i)
	//	image.GaussianBlur5();

	image.Bloom(0.8f, 100, 1.2f);

	image.WritePNG("result.png");

	Initialze(window, width, height, image.GetWidth(), image.GetHeight());
}

void Example::InitShaders()
{
	//Blob 은 이진을 의미
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;

#ifdef _DEBUG
	ComPtr<ID3DBlob> errorBlob;

	// DX는 기본적으로 함수의 반환값이 HRESULT로 성공/실패 여부를 나타낸다.
	// 우리가 만든 vs(꼭짓점 쉐이더파일의 main이 실행되도록 진입점을 설정해줌)
	if (FAILED(D3DCompileFromFile(L"_Shaders/VertexShader.hlsl", 0, 0, "main", "vs_5_0",
		0, 0, &vsBlob, &errorBlob)))
	{
		// 에러가 있는지 확인
		if (errorBlob.Get())
		{
			// 에러를 문자열 형태로 받아옴.
			const std::string& str = (const char*)errorBlob->GetBufferPointer();
			// 에러를 띄워줌
			MessageBoxA(nullptr, str.c_str(), "Vertex Shader Compile Error!", MB_OK);
		}
		// 위 if문에 걸리지 않았더라도, 문제가 있다면 이 if문에 들어오기 때문에
		// assert로 처리해준다.
		assert(false);
	}

	// 위 과정을 vs뿐 아닌 ps에서도 똑같이 해줌.
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

	// 연산자 오버로딩으로 인해 vertexShader는 로포인터가 아닌 이중 포인터로 전달이 됨.
	// 주소값을 넘겨줄 때 해제를 해서 넘겨줌. 주소값만 넘겨주는 함수는 따로 존재함.
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

void Example::Initialze(HWND window, UINT width, UINT height, UINT canvasWidth, UINT canvasHeight)
{
	this->canvasWidth = canvasWidth;
	this->canvasHeight = canvasHeight;

	// DX에선 더블버퍼링이 기본이며, 프론트 버퍼와 백 버퍼를 번갈아 가게 된다.(스왑체인)
	// 백 버퍼가 모두 그려진 후 프론트 버퍼와 교체하는 방식. 그렇기 때문에 빈 버퍼로 인한 깜빡임이 없음.
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
	// 자료형의 id를 알려주는 연산자 __uuidof() 아래에선 매크로 활용
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

	// CreateSampler
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
	textureDesc.Width = canvasWidth;
	textureDesc.Height = canvasHeight;
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
			// 텍스쳐 좌표를 uv좌표에 대응 시켜주는 것.
			// 4개 중 마지막은 포지션, 방향을 나타내는 벡터, 현재 포지션으로 고정
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
		// 삼각형 두 개, 시계방향
		const vector<uint16_t> indices = //16비트 unsigned Short
		{
			3, 1, 0,
			2, 1, 3,
		};

		indexCount = UINT(indices.size());

		D3D11_BUFFER_DESC bufferDese = {};
		bufferDese.ByteWidth = UINT(sizeof(indices[0]) * indices.size());
		bufferDese.Usage = D3D11_USAGE_DYNAMIC;
		bufferDese.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDese.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDese.StructureByteStride = sizeof(indices[0]);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();

		CHECK(device->CreateBuffer(&bufferDese, &indexBufferData, &indexBuffer));
	}
}

void Example::Update()
{
	auto& pixels = image.GetPixels();

	

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	deviceContext->Map(canvasTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	{
		memcpy(mappedSubresource.pData, pixels.data(), sizeof(pixels[0]) * pixels.size());
	}
	deviceContext->Unmap(canvasTexture.Get(), 0);
}

void Example::Render()
{
	float clearColor[4] = { .0f, .0f, .0f, 1.0f };
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);

	//렌더링 파이프라인 순서
	//IA(Input Assembler) - VS(Vertex Shader) - RS(Rasterize) - PS(Pixel Shader) - OM(Output Merge)
	//이 모든 과정을 가동하기 위해선 DrawCall or DPCall을 통해서 하게됨
	//그럼으로 모든 단계에서 필요한 정보를 모두 꽂아준 후(순서 상관X) DrawCall을 통해 렌더링 파이프 라인이 한바퀴를 돌게됨.

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