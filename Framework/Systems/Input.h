#pragma once

#define MAX_INPUT_KEY 256
#define MAX_INPUT_MOUSE 8

class Input
{
	DECLARE_SINGLETON(Input);

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS
	};

	enum
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
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
	void KeyBoard();
	void Mouse();

	void Update();

	void InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	bool Down(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }
	bool DblClk(const DWORD key) const { return isDblClk; }

	Vector3 GetMousePos() const { return mousePos; }

	bool Down(const DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	bool Up(const DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	bool Press(const DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }
	Vector3 GetMoveValue() const { return wheelMoveValue; }

private:
	array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };

	HWND handle;
	Vector3 mousePos;

	byte buttonStatus[MAX_INPUT_MOUSE] = { 0 };
	byte buttonOldStatus[MAX_INPUT_MOUSE] = { 0 };
	byte buttonMap[MAX_INPUT_MOUSE] = { 0 };

	Vector3 wheelStatus;
	Vector3 wheelOldStatus;
	Vector3 wheelMoveValue;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	bool isDblClk = false;
};