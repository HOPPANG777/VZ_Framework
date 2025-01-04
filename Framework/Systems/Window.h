#pragma once

struct WinDesc
{
	wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	UINT width;
	UINT height;
};

class Window
{
public:
	Window(const WinDesc& initDesc);
	~Window();

	ATOM MyRegisterClass(const WinDesc& desc);

	WPARAM Run();

	void MainRender();

	HICON LoadMyIcon();

private:
	static LRESULT CALLBACK WndProc
	(
		//Window handle
		HWND handle,
		//Message Queue에서 받은 메시지
		UINT message,
		//워드 파라미터(정수값)
		WPARAM wParam,
		//롱 파라미터(포인터)
		LPARAM lParam
	);

	WinDesc desc;

	static unique_ptr<class Program> program;
};