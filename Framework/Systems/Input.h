#pragma once
#include "Utilities/DirectHelper.h"

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

public:
	void Update();

	void InputProc(UINT message, LPARAM lParam);

	bool Down(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }
	bool DblClk(const DWORD key) const { return isDblClk; }

	Vector3 GetMousePos() const { return mousePos; }

private:
	Vector3 mousePos;

	array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };

	bool isDblClk = false;
};