#pragma once

enum class AnimState
{
    ANIMATION_NONE = 0,
    ANIMATION_UP,
    ANIMATION_DOWN,
    ANIMATION_LEFT,
    ANIMATION_LEFT_UP,
    ANIMATION_LEFT_DOWN,
    ANIMATION_RIGHT,
    ANIMATION_RIGHT_UP,
    ANIMATION_RIGHT_DOWN
};

// Drawable을 상속해 실제 그리는 Rect
class AnimationRect : public Drawable, public IColorable
{
private:
    AnimState animState = AnimState::ANIMATION_LEFT;

    vector<VertexTexture> vertices;
    vector<UINT> indices;

    // RS, Blend, Sampler 등 D3D 리소스
    ComPtr<ID3D11RasterizerState> rs = nullptr;
    ComPtr<ID3D11BlendState> blend[2];
    ComPtr<ID3D11SamplerState> sampler[2];

    // 최종적으로 픽셀 셰이더에 전달할 SRV
    ComPtr<ID3D11ShaderResourceView> srv = nullptr;

    D3D11_MAPPED_SUBRESOURCE subRe;

    // 컬러 변환을 위한 상수 버퍼 (ColorBuffer)
    unique_ptr<ColorBuffer> colorBuffer;

public:
    AnimationRect(const Vector3& position, const Vector3& scale, float rotation, const std::wstring path = L"");
    virtual ~AnimationRect();

public:
    void Update() override;
    void Render() override;

public:
    // 필요 시 직접 SRV를 설정
    void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

    // 외부에서 이미지 교체
    void SetImage(const std::wstring& imagePath)
    {
        HRESULT hr = D3DX11CreateShaderResourceViewFromFile
        (
            DEVICE,
            imagePath.c_str(),
            nullptr,
            nullptr,
            srv.GetAddressOf(),
            nullptr
        );
        CHECK(hr);
    }

public:
    virtual Color GetColor() const override { return colorBuffer->GetColor();; }

    virtual void SetColor(const Color& color) override
    {
        colorBuffer->SetColor(color);
        // 별도로 정점 버퍼의 데이터를 변경할 필요는 없습니다.
        // 픽셀 셰이더에서는 상수 버퍼의 색상 값을 사용하여 텍스처 색상을 변경합니다.
    }

public:
    // 필요 시 Animator를 직접 얻어올 수 있는 헬퍼함수
    shared_ptr<Animator> GetAnimator() { return GetComponent<Animator>("Animator"); }
    
};