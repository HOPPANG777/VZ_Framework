#pragma once
#include "Framework.h"
#include "Utilities/DirectHelper.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Interfaces/IInput.h"

#define MAX_INPUT_KEY 256

#define MAX_INPUT_MOUSE 8

class Input : public IInput
{
    DECLARE_SINGLETON(Input);

public:
    // 매 프레임마다 키보드와 마우스 업데이트 호출
    void Update() override
    {
        KEYBOARD->Update();
        MOUSE->Update();
    }

    // Keyboard 관련 래퍼 함수들
    bool KeyDown(uint32_t key) const override { return KEYBOARD->Down(key); }
    bool KeyUp(uint32_t key) const override { return KEYBOARD->Up(key); }
    bool KeyPressed(uint32_t key) const override { return KEYBOARD->Press(key); }

    // Mouse 관련 래퍼 함수들
    Vector3 GetMousePos() const override { return MOUSE->GetMousePos(); }
    bool MouseDown(uint32_t button) const override { return MOUSE->Down(button); }
    bool MouseUp(uint32_t button) const override { return MOUSE->Up(button); }
    bool MousePressed(uint32_t button) const override { return MOUSE->Press(button); }
    LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam) const { return MOUSE->MouseProc(message, wParam, lParam); }
    Vector3 GetMouseMove() const override { return MOUSE->GetMoveValue(); }

};