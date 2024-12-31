#include "Framework.h"
#include "Input.h"

Input::Input() {}

Input::~Input() {}

void Input::Update()
{
	Mouse();
	KeyBoard();
}

void Input::KeyBoard()
{
	keyOldState = move(keyState);

	if (GetKeyboardState(keyState.data()))
	{
		for (DWORD i = 0; i < MAX_INPUT_KEY; ++i)
		{
			keyState[i] = keyState[i] & 0x80 ? 1 : 0;

			auto& oldState = keyOldState[i];
			auto& state = keyState[i];

			if (oldState == 0 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_DOWN;
			}
			else if (oldState == 1 && state == 0)
			{
				keyMap[i] = KEY_INPUT_STATUS_UP;
			}
			else if (oldState == 1 && state == 1)
			{
				keyMap[i] = KEY_INPUT_STATUS_PRESS;
			}
			else
			{
				keyMap[i] = KEY_INPUT_STATUS_NONE;
			}
		}
	}
}

void Input::Mouse()
{
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1) buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0) buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1) buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(handle, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = float(point.x);
	wheelStatus.y = float(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus;

	DWORD tButtonStatus = GetTickCount();
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonMap[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1) startDblClk[i] = tButtonStatus;
		}

		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk) buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}

void Input::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		mousePos.x = (float)LOWORD(lParam);
		mousePos.y = (float)(gWinHeight - HIWORD(lParam));
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += (float)tWheelValue;
	}
}