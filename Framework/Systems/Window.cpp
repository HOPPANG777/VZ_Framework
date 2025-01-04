#include "Framework.h"
#include "Window.h"
#include "Resources/resource.h"
#include "../UnitTest/Program.h"

unique_ptr<Program> Window::program = nullptr;

Window::Window(const WinDesc& initDesc)
	: desc(initDesc)
{
	WORD wHr = MyRegisterClass(desc);
	assert(wHr != 0);

	desc.handle = CreateWindowExW
	(
		WS_EX_APPWINDOW,
		desc.appName.c_str(),
		desc.appName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		HMENU(nullptr),
		desc.instance,
		nullptr
	);

	RECT rect = { 0, 0, LONG(desc.width), LONG(desc.height) };

	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

	UINT centerLeft = (GetSystemMetrics(SM_CXSCREEN) - UINT(desc.width)) / 2 + rect.left;
	UINT centerTop = (GetSystemMetrics(SM_CYSCREEN) - UINT(desc.height)) / 2 + rect.top;

	MoveWindow
	(
		desc.handle,
		centerLeft,
		centerTop,
		rect.right - rect.left,
		rect.bottom - rect.top,
		true
	);

	ShowWindow(desc.handle, SW_SHOWNORMAL);

	UpdateWindow(desc.handle);

	ShowCursor(true);
}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClassW(desc.appName.c_str(), desc.instance);
}

ATOM Window::MyRegisterClass(const WinDesc& desc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	wcex.lpfnWndProc = &WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = desc.instance;
	wcex.hIcon = LoadIcon(desc.instance, MAKEINTRESOURCE(IDI_HOPPANG));	//LoadMyIcon();	//LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(GetStockObject(GRAY_BRUSH));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = desc.appName.c_str();
	wcex.hIconSm =  LoadIcon(desc.instance, MAKEINTRESOURCE(IDI_SMALLPPANG)); //wcex.hIcon;

	return RegisterClassExW(&wcex);
}

WPARAM Window::Run()
{
	program = make_unique<Program>();
	program->Start();

	MSG msg = { 0 };
	
	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 메시지 종료 함수 반환시 while문 탈출
			if (msg.message == WM_QUIT) break; 

			// 메세지 변역
			TranslateMessage(&msg);

			//INPUT->InputProc(msg.message, msg.lParam);
			MOUSE->MouseProc(msg.message, msg.wParam, msg.lParam);
			if (IMGUI->MsgProc(msg.hwnd, msg.message, msg.wParam, msg.lParam)) return true;

			// 메시지 처리함수로 전달
			DispatchMessage(&msg);
		}
		else
		{
			// 업데이트와 렌더
			MainRender();
		}
	}

	return msg.wParam;
}

void Window::MainRender()
{
	//INPUT->Update();
	KEYBOARD->Update();
	MOUSE->Update();
	TIME->Update();
	SOUNDS->Update();
	IMGUI->Update();

	program->Update();

	GRAPHICS->Begin();
	{
		program->Render();
		program->PostRender();

		program->ImGUI();
		IMGUI->Render();
	}
	GRAPHICS->End();
}

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
	case WM_CREATE:
		gHandle = handle;
		break;
	case WM_SIZE:
		gWinWidth = float(GET_X_LPARAM(lParam));
		gWinHeight = float(GET_Y_LPARAM(lParam));
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		//if (MessageBox(handle, L"정말 종료하시겠습니까?", L"WinAPI 종료", MB_OKCANCEL) == IDOK)
		PostQuitMessage(0);

		return 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

// 아이콘 리소스를 로드 (리소스(.rc) 파일 또는 ico 파일에서)
HICON Window::LoadMyIcon()
{
	// 방법 1) 리소스 (.rc) 기반 아이콘
	// return static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MY_ICON), IMAGE_ICON, 32, 32, 0));
	// return static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MY_ICON), IMAGE_ICON, 32, 32, 0));
	// return static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MY_ICON), IMAGE_ICON, 32, 32, 0));

	// 방법 2) 외부 ico 파일 로드
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/Ico/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));			// HOPPANG 아이콘
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/ico_Image/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/ico_Image/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/ico_Image/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/ico_Image/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
	//return static_cast<HICON>(LoadImage(NULL, L"../_Textures/ico_Image/HOPPANG.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
}
