#include "Framework.h"
#include "Texture2D.h"

// Texture2D 생성자
Texture2D::Texture2D(const wstring& filePath)
    : filePath(filePath)
{
    TEXTURE2D->Load(this);
}

// ReadPixel 멤버 함수
void Texture2D::ReadPixel(vector<Color>* pixels)
{
    // srv에서 실제 Texture2D COM 포인터 가져오기
    ComPtr<ID3D11Resource> resource;
    srv->GetResource(&resource);

    ComPtr<ID3D11Texture2D> texture2D;
    resource.As(&texture2D);

    ReadPixel(texture2D, pixels);
}

// ReadPixel 정적 함수
void Texture2D::ReadPixel(ComPtr<ID3D11Texture2D> texture, vector<Color>* pixels)
{
    // 원본 텍스처 정보
    D3D11_TEXTURE2D_DESC srvDesc;
    texture->GetDesc(&srvDesc);

    // 복사본 desc 설정
    D3D11_TEXTURE2D_DESC destDesc = {};
    destDesc.Usage = D3D11_USAGE_STAGING;
    destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    destDesc.Width = srvDesc.Width;
    destDesc.Height = srvDesc.Height;
    destDesc.MipLevels = 1;
    destDesc.ArraySize = 1;
    destDesc.Format = srvDesc.Format;
    destDesc.SampleDesc = srvDesc.SampleDesc;

    // CreateTexture2D
    ComPtr<ID3D11Texture2D> destTex;
    HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, &destTex);
    CHECK(hr);

    // 원본 texture를 복사본 destTex에 로드
    hr = D3DX11LoadTextureFromTexture(DC, texture.Get(), nullptr, destTex.Get());
    CHECK(hr);

    // 픽셀 데이터 담을 메모리(Width * Height)
    const size_t pixelCount = static_cast<size_t>(destDesc.Width) * static_cast<size_t>(destDesc.Height);

    // unique_ptr로 배열 관리
    const unique_ptr<UINT[]>& colors = make_unique<UINT[]>(pixelCount);

    // Map을 통해 CPU로 읽어온다
    D3D11_MAPPED_SUBRESOURCE subRe;
    DC->Map(destTex.Get(), 0, D3D11_MAP_READ, 0, &subRe);
    {
        // 복사
        memcpy(colors.get(), subRe.pData, sizeof(UINT) * pixelCount);
    }
    DC->Unmap(destTex.Get(), 0);

    // OpenGL 호환용(BGR → RGB 순서 교체)
    {
        // 임시 버퍼들
        const unique_ptr<UINT[]>& alpha = make_unique<UINT[]>(pixelCount);
        const unique_ptr<UINT[]>& red = make_unique<UINT[]>(pixelCount);
        const unique_ptr<UINT[]>& green = make_unique<UINT[]>(pixelCount);
        const unique_ptr<UINT[]>& blue = make_unique<UINT[]>(pixelCount);

        for (size_t i = 0; i < pixelCount; i++)
        {
            alpha[i] = (colors[i] & 0xFF000000);
            red[i] = (colors[i] & 0x00FF0000);
            green[i] = (colors[i] & 0x0000FF00);
            blue[i] = (colors[i] & 0x000000FF);

            // R과 B 위치 교체
            red[i] = colors[i] >> 16;    // 16비트 쉬프트 → R 추출
            blue[i] = colors[i] << 16;    // <<16 → B를 상위 비트로

            colors[i] = alpha[i] | red[i] | green[i] | blue[i];
        }

        // 최종 픽셀을 결과 벡터에 저장
        pixels->assign(colors.get(), colors.get() + pixelCount);
    }
}

