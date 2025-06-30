#include "Framework.h"
#include "Window.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Systems/Sounds.h"
#include "Utilities/ServiceRegister.h"
#include "../Client/Program.h"

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
	wcex.hIcon = desc.icon;	 //LoadIcon(desc.instance, MAKEINTRESOURCE(IDI_HOPPANG));	//LoadMyIcon();	//LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = desc.appName.c_str();
	wcex.hIconSm = wcex.hIcon;//LoadIcon(desc.instance, MAKEINTRESOURCE(IDI_SMALLPPANG)); //

	return RegisterClassExW(&wcex);
}

WPARAM Window::Run()
{
        RegisterServices();
        program = make_unique<Program>();
        program->Awake();
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
			INPUT->MouseProc(msg.message, msg.wParam, msg.lParam);
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
	INPUT->Update();
	TIME->Update();
	SOUNDS->Update();
	IMGUI->Update();
	COLLISION->Update();

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

void Window::SetBorderlessWindow()
{
	DWORD style = WS_POPUP | WS_VISIBLE;
	SetWindowLong(desc.handle, GWL_STYLE, style);

	SetWindowPos
	(
		desc.handle, HWND_TOP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED
	);
}

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGui가 메시지를 처리했다면 바로 반환
	//if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam)) return 1;

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

