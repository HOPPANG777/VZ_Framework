#pragma once

#include "Utilities/DirectHelper.h"

#define MAX_INPUT_MOUSE 8

class Mouse
{
	DECLARE_SINGLETON(Mouse);

	enum
	{
		MOUSE_ROTATION_RIGHT = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_WHEEL
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

public:
	void Init();

	void Update();
	LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam);

	Vector3 GetMousePos() const { return mousePos; }

	bool Down(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	bool Up(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	bool Press(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }

	Vector3 GetMoveValue() const { return wheelMoveValue; }

private:
	HWND handle;
	Vector3 mousePos;

	std::array<unsigned char, MAX_INPUT_MOUSE> buttonStatus = { 0 };
	std::array<unsigned char, MAX_INPUT_MOUSE> buttonOldStatus = { 0 };
	std::array<unsigned char, MAX_INPUT_MOUSE> buttonMap = { 0 };

	Vector3 wheelStatus = Values::ZeroVec3;
	Vector3 wheelOldStatus = Values::ZeroVec3;
	Vector3 wheelMoveValue = Values::ZeroVec3;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

};

