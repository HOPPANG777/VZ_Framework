#pragma once
#include "Utilities/DirectHelper.h"
#include "Keyboard.h"
#include "Mouse.h"

#define MAX_INPUT_KEY 256

#define MAX_INPUT_MOUSE 8

class Input
{
    DECLARE_SINGLETON(Input);

public:
    // 매 프레임마다 키보드와 마우스 업데이트 호출
    void Update()
    {
        Keyboard::Get()->Update();
        Mouse::Get()->Update();
    }

    // Keyboard 관련 래퍼 함수들
    bool KeyDown(DWORD key) const { return Keyboard::Get()->Down(key); }
    bool KeyUp(DWORD key) const { return Keyboard::Get()->Up(key); }
    bool KeyPressed(DWORD key) const { return Keyboard::Get()->Press(key); }

    // Mouse 관련 래퍼 함수들
    DirectX::SimpleMath::Vector3 GetMousePos() const { return Mouse::Get()->GetMousePos(); }
    bool MouseDown(DWORD button) const { return Mouse::Get()->Down(button); }
    bool MouseUp(DWORD button) const { return Mouse::Get()->Up(button); }
    bool MousePressed(DWORD button) const { return Mouse::Get()->Press(button); }
    LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam) const { return Mouse::Get()->MouseProc(message, wParam, lParam); }
    DirectX::SimpleMath::Vector3 GetMouseMove() const { return Mouse::Get()->GetMoveValue(); }

};