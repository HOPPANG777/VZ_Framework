#include "stdafx.h"
#include "DialogTextBox.h"

DialogTextBox::DialogTextBox(const string& name, const Vector3& position, const Vector3& size, const wstring& backgroundTexturePath, const wstring& arrowTexturePath)
    : Object(name, position, size, 0.0f, TransformType::Rect)
{
    // 대화창 배경 생성 (배경 텍스처 사용)
    background = make_shared<TextureRect>(position, size, 0.0f, backgroundTexturePath);

    // 텍스트 객체 생성: 대화창 내 텍스트 영역 (예: 전체 크기의 일부 영역)
    // 여기서는 배경 위에 겹쳐서 표시합니다.
    Vector3 textPos = position; // 필요에 따라 오프셋 적용
    Vector3 textSize = size;
    textObject = make_shared<Object>("DialogText", textPos, textSize, 0.0f, TransformType::Rect);

    // TypingText 컴포넌트를 추가 (타이핑 효과 속도는 0.05초 정도)
    auto typing = make_shared<TypingText>("TypingText", 0.05f);
    textObject->AddComponent(typing);

    // 초기 텍스트는 빈 문자열로 설정
    typing->SetText(L"");
    textComplete = false;

    // 화살표 아이콘 생성: 대화창의 오른쪽 아래 모서리에 위치, 기본적으로 숨김 처리(렌더링 시 조건 처리)
    Vector3 arrowPos = { position.x + size.x - 30, position.y + 5, position.z };
    Vector3 arrowSize = { 20, 20, 1 };
    arrowIndicator = make_shared<TextureRect>(arrowPos, arrowSize, 0.0f, arrowTexturePath);
}

void DialogTextBox::SetDialogText(const wstring& text)
{
    dialogText = text;
    // 텍스트 객체의 TypingText 컴포넌트에 새 텍스트 설정 및 타이핑 효과 재시작
    auto typing = textObject->GetComponent<TypingText>("TypingText");

    if (typing)
    {
        typing->SetText(dialogText);
        textComplete = false;
    }
}

void DialogTextBox::Update()
{
    if (!active) return;

    // 텍스트 객체 업데이트 (타이핑 효과 진행)
    if (textObject)
    {
        textObject->Update();
        auto typing = textObject->GetComponent<TypingText>("TypingText");
        if (typing && typing->IsComplete())
        {
            textComplete = true;
        }
    }

    // 텍스트 출력이 완료되면 화살표 아이콘 업데이트 및 클릭 입력 처리
    if (textComplete && arrowIndicator)
    {
        arrowIndicator->Update();

        // 마우스 입력 검사: 왼쪽 클릭 시, 마우스 포인터가 화살표 영역에 있으면 대화창 종료
        if (INPUT->MousePressed(0))
        {
            Vector3 mousePos = INPUT->GetMousePos() + CAMERA->GetPosition();
            RECT arrowRect = arrowIndicator->GetRect();
            if (PointInRect(mousePos, arrowRect))
            {
                Deactivate();
            }
        }
    }
}

void DialogTextBox::Render()
{
    if (!active) return;

    // Render 순서: 배경 → 텍스트 → (텍스트 출력 완료시) 화살표
    if (background) background->Render();
    if (textObject) textObject->Render();
    if (textComplete && arrowIndicator) arrowIndicator->Render();
}
