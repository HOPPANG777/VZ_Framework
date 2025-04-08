#include "Framework.h"
#include "Button.h"

void Button::Update()
{
	bool over = IsMouseOver();         // 버튼 영역 내에 마우스 존재 여부
	bool pressedNow = INPUT->MousePressed(0);   // 현재 프레임에 버튼이 눌린 상태
	bool downNow = INPUT->MouseDown(0);    // 이번 프레임에 새롭게 눌림
	bool upNow = INPUT->MouseUp(0);        // 이번 프레임에 버튼을 뗀 상태

	IColorable* colorableOwner = dynamic_cast<IColorable*>(GetOwner());

	if (over)
	{
		// 색상 효과: 버튼이 눌려있거나 단일 클릭 효과 시간이 남아 있다면 어둡게 표시
		if (pressedNow || clickEffectTime > 0.0f)
		{
			colorableOwner->SetColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
		}
		else
		{
			colorableOwner->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
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
			wasPressed = false; // 눌림 상태 초기화

			if (onClick) onClick();
		}

		// onPress: 누르고 있는 동안 매 프레임 호출 (등록된 경우)
		if (onPress && pressedNow) onPress();
	}
	else
	{
		colorableOwner->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		clickEffectTime = 0.0f;
		wasPressed = false; // 영역을 벗어나면 초기화
	}

	// 타이머 업데이트: 만약 효과 시간이 남아 있다면 감소
	if (clickEffectTime > 0.0f) clickEffectTime -= DELTA;
}

void Button::Render()
{

}

bool Button::IsMouseOver()
{
	// Button이 속한 Object에 RectTransform이 있다고 가정
	std::shared_ptr<RectTransform> rectTrans = GetOwner()->GetComponent<RectTransform>("RectTransform");
	if (!rectTrans) return false;

	// RectTransform에서 픽셀 좌표 RECT 가져오기
	RECT rect = rectTrans->GetRect();

	// 마우스 현재 위치
	Vector3 mousePos = INPUT->GetMousePos();
	//  (오쏘그래픽 매트릭스나 기타 변환이 있다면 별도 보정 필요)

	// RECT 내부 판정
	bool inside = (mousePos.x >= rect.left && mousePos.x <= rect.right && mousePos.y >= rect.top && mousePos.y <= rect.bottom);

	return inside;
}