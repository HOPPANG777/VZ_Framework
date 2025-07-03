#pragma once

#include "Objects/BaseObject/Object.h"
#include "Objects/TextureObject/TextureRect.h"
#include "Components/TypingText.h"
#include "Systems/Input.h"

class DialogTextBox : public Object
{
public:
    DialogTextBox(const std::string& name, const Vector3& position, const Vector3& texturePosition, const Vector3& scale, const float rotation, const std::wstring& backgroundTexturePath, const std::wstring& arrowTexturePath, const std::wstring& text, const float fontSize = 25.0f , const std::wstring& font = L"Pretendard JP", const Color& fontColor = BLACK);
    virtual ~DialogTextBox() = default;

public:
    virtual void Update() override;
    virtual void Render() override;
	virtual void PostRender();

public:
    // 대화창 활성/비활성 상태 제어
    void Activate() { active = true; }
    void Deactivate() { active = false; }

private:
    // 대화 텍스트
    std::shared_ptr<Object> background;
    std::shared_ptr<Object> arrowIndicator;
	std::shared_ptr<Object> playerTexture;

    // 배경, 텍스트, 화살표 아이콘 객체
    std::wstring dialogText;
	std::wstring fontName;
	Color fontColor;
    float fontSize;
    bool textComplete = false;
	bool arrowVisible = false;
    bool active = true;
};
