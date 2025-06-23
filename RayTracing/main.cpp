// 레이트레이싱 실행을 위한 메인 파일
#include "pch.h"
#include "Example.h"

#define STB_IMAGE_IMPLEMENTATION

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    const UINT width = 1280, height = 720;

    WNDCLASSEX winClass =
    {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
        &WndProc,
        0,
        0,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        L"Graphics",
        nullptr,
    };

    RegisterClassEx(&winClass);

    HWND hwnd = CreateWindow
    (
        winClass.lpszClassName,
        L"Graphics Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        winClass.hInstance,
        nullptr
    );

    RECT winRect = { 0, 0, LONG(width), LONG(height) };

    AdjustWindowRectEx(&winRect, WS_OVERLAPPEDWINDOW, false, 0);

    UINT centerLeft = (GetSystemMetrics(SM_CXSCREEN) - UINT(width)) / 2 + winRect.left;
    UINT centerTop = (GetSystemMetrics(SM_CYSCREEN) - UINT(height)) / 2 + winRect.top;

    MoveWindow
    (
        hwnd,
        centerLeft,
        centerTop,
        winRect.right - winRect.left,
        winRect.bottom - winRect.top,
        true
    );

    ShowWindow(hwnd, SW_SHOWNORMAL);

    UpdateWindow(hwnd);

    ShowCursor(true);

    auto example = make_unique<Example>(hwnd, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
    ImGui::StyleColorsDark();

    ImGui_ImplDX11_Init(example->GetDevice(), example->GetSDeviceContext());
    ImGui_ImplWin32_Init(hwnd);

    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            example->Update();
            example->Render();

            ImGui::Render();

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            example->GetSwapChain()->Present(1, 0);
        }
    }

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(hwnd);
    UnregisterClass(winClass.lpszMenuName, winClass.hInstance);

    return int(msg.wParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(handle, message, wParam, lParam);;
}
