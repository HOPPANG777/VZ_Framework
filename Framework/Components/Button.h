#pragma once

#include "Objects/BaseObject/Object.h"
#include "Component.h"
#include "RectTransform.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Interfaces/IColorable.h"

class Button : public Component
{
public:
	Button(const std::string& name = "Button") : Component(name) {}
	~Button() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

public:
	// 외부에서 콜백을 등록할 수 있도록 SetOnClick 함수를 제공
	void SetOnClick(const std::function<void()>& callback) { onClick = callback; }
	void SetOnPress(const std::function<void()>& callback) { onPress = callback; }

private:
	bool IsMouseOver(); // 내부에서 마우스가 버튼 위에 있는지 판단

private:
	// 클릭 이벤트를 처리할 콜백 함수
	std::function<void()> onClick = nullptr;
	std::function<void()> onPress = nullptr;

	// 클릭 여부를 관리할 간단한 상태 변수
	bool wasPressed = false; // 이전 프레임에서 마우스를 누르고 있었는지

	// 클릭 한번 씩만.
	bool clickLock = false;

	float clickEffectTime = 0.0f; // 단일 클릭 시 효과 지속 시간 (초 단위)

};