#include "stdafx.h"
#include "DialogTextBox.h"

DialogTextBox::DialogTextBox(const string& name, const Vector3& position, const Vector3& scale, const float& rotation, const wstring& backgroundTexturePath, const wstring& arrowTexturePath, const wstring& text, const float& fontSize, const wstring& font, const Color& fontColor)
	: Object(name, position, scale, rotation), dialogText(text), fontSize(fontSize), fontName(font), fontColor(fontColor)
{
    // 배경 생성
    background = make_shared<TextureRect>(position, scale, rotation, backgroundTexturePath, TransformType::Rect);

    // TypingText 컴포넌트 추가 (타이핑 효과)
    auto typing = make_shared<TypingText>("TypingText", 0.075f);
    this->AddComponent(typing);

    // 화살표 인디케이터 생성 (배경 오른쪽 아래 위치에)
    Vector3 arrowPos = position + Vector3(scale.x * 0.4f, -scale.y * 0.4f, 0.0f);
    arrowIndicator = make_shared<TextureRect>(arrowPos, Vector3(30, 30, 0), TexturePath + L"UI/Arrow.png");
    arrowIndicator->SetColor(WHITE);
}

void DialogTextBox::Update()
{
	if (!active) return;

    auto typing = GetComponent<TypingText>("TypingText");
    if (!typing) return;

    // 사용자가 클릭하면...
    if (INPUT->MousePressed(0))
    {
        // 만약 아직 텍스트가 모두 출력되지 않았다면 타이핑 효과 건너뛰기
        if (!typing->IsComplete()) typing->SkipToEnd();
        else active = false; // 이미 텍스트가 모두 출력되었으면 창 닫힘
    }

    // TypingText 업데이트
    typing->Update();

    // 텍스트 출력이 완료되면 화살표 표시
    if (typing->IsComplete()) arrowVisible = true;
}

void DialogTextBox::Render()
{
	if (!active) return;

    // 배경 렌더링
    background->Render();

    // TypingText의 RenderText를 통해 텍스트 출력 (항상 RenderText를 사용)
    auto typing = GetComponent<TypingText>("TypingText");
    if (typing) typing->RenderText(dialogText, this->GetRect(), fontSize, fontColor, fontName);

    // 텍스트 출력 완료 시 화살표 렌더링
    if (arrowVisible && arrowIndicator) arrowIndicator->Render();
}
