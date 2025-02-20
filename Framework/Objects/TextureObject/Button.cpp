#include "Framework.h"
#include "Button.h"

Button::Button(const Vector3& position, const Vector3& scale, const float rotation, const wstring path, const wstring shaderPath)
	: Drawable("Button", position, scale, rotation, TransformType::Rect, shaderPath)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		// 좌 하단
		vertices[0].position = { -0.5f, -0.5f, 0 };
		vertices[0].uv = Vector2(0, 1);

		// 좌 상단
		vertices[1].position = { -0.5f, 0.5f, 0 };
		vertices[1].uv = Vector2(0, 0);

		// 우 하단
		vertices[2].position = { 0.5f, -0.5f, 0 };
		vertices[2].uv = Vector2(1, 1);

		// 우 상단
		vertices[3].position = { 0.5f, 0.5f, 0 };
		vertices[3].uv = Vector2(1, 0);
	}

	//vertexBuffer
	{
		//객체를 만들어 주어야 한다.
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//indexBuffer
	{
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//inputLayout
	{
		inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());
	}

	// Rasterizer State
	// 레스터라이저는 버텍스 쉐이더로 찍은 정점들의 사이와 내부를
	// 채워주는 역할이다.
	// 이거 안만들면 기본값으로 넣어주고 만들면 만든거 넣어줌
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		// CullMode : 그리지 않을 부분 설정
		// d3d11_cull_back == 뒷면 지우겠다. (반시계 방향 안그림)
		desc.CullMode = D3D11_CULL_BACK;
		//desc.CullMode = D3D11_CULL_NONE;

		// FillMode : 체우는 방식 설정
		// d3d11_fill_solid == 전부 체우겠다. (빈공간 안만든다)
		desc.FillMode = D3D11_FILL_SOLID;

		// 시계방향 설정 (false == 시계 방향 // true == 반 시계 방향)
		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rs);
		CHECK(hr);
	}

	// SRV 추출 // 그림 데이터 추출
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			path.c_str(),
			nullptr,
			nullptr,
			srv.GetAddressOf(),
			nullptr
		);
		CHECK(hr);
	}


	// Sampler
	{
		D3D11_SAMPLER_DESC desc;
		RenderStates::GetSamplerDesc(&desc);
		RenderStates::CreateSampler(&desc, &sampler[0]);

		// 선형보간
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		RenderStates::CreateSampler(&desc, &sampler[1]);
	}

	// Blend
	{
		D3D11_BLEND_DESC desc;
		RenderStates::GetBlendDesc(&desc);
		RenderStates::CreateBlend(&desc, &blend[0]);

		desc.RenderTarget[0].BlendEnable = true;
		RenderStates::CreateBlend(&desc, &blend[1]);
	}

	// ColorBuffer 생성
	{
		colorBuffer = make_unique<ColorBuffer>();
	}
}

Button::~Button()
{
}

void Button::Update()
{
	BASE::Update();

	bool over = IsMouseOver();         // 버튼 영역 내에 마우스 존재 여부
	bool pressedNow = MOUSE->Press(0);   // 현재 프레임에 버튼이 눌린 상태
	bool downNow = MOUSE->Down(0);    // 이번 프레임에 새롭게 눌림
	bool upNow = MOUSE->Up(0);        // 이번 프레임에 버튼을 뗀 상태

	if (over)
	{
		// 색상 효과: 버튼이 눌려있거나 단일 클릭 효과 시간이 남아 있다면 어둡게 표시
		if (pressedNow || clickEffectTime > 0.0f)
		{
			SetColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
		}
		else
		{
			SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 마우스를 누르고 있는 동안
		if (downNow)
		{
			clickEffectTime = 0.1f; // 단일 클릭 시 효과 (예: 0.1초)
			wasPressed = true; // 눌렸다는 상태 저장
		}

		// 마우스를 눌렀다가 뗄 때 onClick 실행
		if (upNow && wasPressed)
		{
			if (onClick) onClick();
			wasPressed = false; // 눌림 상태 초기화
		}

		// onPress: 누르고 있는 동안 매 프레임 호출 (등록된 경우)
		if (onPress && pressedNow) onPress();
	}
	else
	{
		SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		clickEffectTime = 0.0f;
		wasPressed = false; // 영역을 벗어나면 초기화
	}

	// 타이머 업데이트: 만약 효과 시간이 남아 있다면 감소
	if (clickEffectTime > 0.0f) clickEffectTime -= DELTA;
}


void Button::Render()
{
    BASE::Render();

	// 먼저, 픽셀 셰이더의 상수 버퍼 슬롯(예: b2)에 ColorBuffer를 업데이트 및 바인딩합니다.
	colorBuffer->SetPSBuffer(2);  // b2 슬롯에 바인딩 (셰이더 코드와 일치해야 함)

	DC->PSSetSamplers(0, 1, &sampler[1]);

	//// SRV를 픽셀 셰이더에 바인딩
	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
	// 레스터라이저 및 블렌드 상태 설정
	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);

	// 드로우 호출
	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Button::IsMouseOver()
{
	// Button이 속한 Object에 RectTransform이 있다고 가정
	auto rectTrans = this->GetComponent<RectTransform>("RectTransform");
	if (!rectTrans) return false;

	// RectTransform에서 픽셀 좌표 RECT 가져오기
	RECT rect = rectTrans->GetRect();

	// 마우스 현재 위치
	Vector3 mousePos = MOUSE->GetMousePos();
	//  (오쏘그래픽 매트릭스나 기타 변환이 있다면 별도 보정 필요)

	// RECT 내부 판정
	bool inside = (mousePos.x >= rect.left && mousePos.x <= rect.right && mousePos.y >= rect.top && mousePos.y <= rect.bottom);

	return inside;
}