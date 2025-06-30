#pragma once
#include "Framework.h"

#define MAX_INPUT_KEY 256

class Keyboard
{
	DECLARE_SINGLETON(Keyboard);

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS
	};

public:
	void Update();

	bool Down(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

private:
	array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
	array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };

};

