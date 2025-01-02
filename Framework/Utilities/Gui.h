#pragma once

class Gui
{
	DECLARE_SINGLETON(Gui);
public:
	void Init();

	LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void Update();
	void Render();

private:
	void ApplyStyle();

};

