#pragma once
#include "Structs/WinDesc.h"
#include "Utilities/Gui.h"
#include "Interfaces/IWindow.h"

// Windows 플랫폼 전용 창 구현체입니다.
class Window : public IWindow
{
public:
	Window(const WinDesc& initDesc);   // 생성자
	~Window();                         // 소멸자

	ATOM MyRegisterClass(const WinDesc& desc); // 윈도우 클래스 등록

	WPARAM Run() override;                     // 메시지 루프 실행

	void MainRender();                         // 프레임 렌더링

	void SetBorderlessWindow();                // 전체 화면 무테두리 창으로 전환

	//HICON LoadMyIcon();

private:
	static LRESULT CALLBACK WndProc
	(
		HWND handle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	WinDesc desc;

	static std::unique_ptr<class Program> program;
};
