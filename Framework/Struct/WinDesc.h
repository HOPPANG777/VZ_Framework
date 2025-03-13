#pragma once

struct WinDesc
{
	wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	UINT width;
	UINT height;
	HICON icon = nullptr;
};