#include "stdafx.h"
#include "DialogTextBox.h"

DialogTextBox::DialogTextBox(const string& name, const Vector3& position, const Vector3& texturePosition, const Vector3& scale, const float rotation, const wstring& backgroundTexturePath, const wstring& arrowTexturePath, const wstring& text, const float fontSize, const wstring& font, const Color& fontColor)
	: Object(name, position, scale, rotation, TransformType::Rect), dialogText(text), fontSize(fontSize), fontName(font), fontColor(fontColor)
{
    // 배경 생성
    background = make_shared<TextureRect>(texturePosition, scale + Vector3(200, 0, 0), rotation, backgroundTexturePath, TransformType::Rect);

	playerTexture = make_shared<TextureRect>(texturePosition + Vector3(-400, 100, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"UI/Player.png");

    // TypingText 컴포넌트 추가 (타이핑 효과)
    std::shared_ptr<TypingText> typing = make_shared<TypingText>();
    this->AddComponent(typing);

    // 화살표 인디케이터 생성 (배경 오른쪽 아래 위치에)
    Vector3 arrowPos = texturePosition + Vector3((scale.x + 200) * 0.45f, -scale.y * 0.18f, 0.0f);
    arrowIndicator = make_shared<TextureRect>(arrowPos, Vector3(25, 25, 0), 0.0f, TexturePath + L"UI/Arrow.png");
}

void DialogTextBox::Update()
{
	if (!active) return;

	BASE::Update();

    background->Update();

    arrowIndicator->Update();

	playerTexture->Update();

    std::shared_ptr<TypingText> typing = this->GetComponent<TypingText>("TypingText");

    // 사용자가 클릭하면...
    if (Input::Get()->MouseDown(0))
    {
        // 만약 아직 텍스트가 모두 출력되지 않았다면 타이핑 효과 건너뛰기
        if (!typing->IsComplete()) typing->SkipToEnd();
        else
        {
            active = false; // 이미 텍스트가 모두 출력되었으면 창 닫힘
            typing->SetTextOut(true);
        }
    }

    // TypingText 업데이트
    typing->Update();

    // 텍스트 출력이 완료되면 화살표 표시
    if (typing->IsComplete()) arrowVisible = true;
}

void DialogTextBox::Render()
{
	if (!active) return;

	BASE::Render();

	playerTexture->Render();

    // 배경 렌더링
    background->Render();

    // 텍스트 출력 완료 시 화살표 렌더링
    if (arrowVisible && arrowIndicator) arrowIndicator->Render();
}

void DialogTextBox::PostRender()
{
	if (!active) return;

    // TypingText의 RenderText를 통해 텍스트 출력 (항상 RenderText를 사용)
    std::shared_ptr<TypingText> typing = this->GetComponent<TypingText>("TypingText");
    typing->RenderText(dialogText, this->GetRect(), fontSize, fontColor, fontName);
}
