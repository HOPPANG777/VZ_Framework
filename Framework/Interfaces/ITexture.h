#pragma once
#include <string>

// 텍스처 공통 인터페이스입니다.
class ITexture
{
public:
    virtual ~ITexture() = default;

    // 파일 경로 반환
    virtual std::wstring GetFilePath() const = 0;
};
