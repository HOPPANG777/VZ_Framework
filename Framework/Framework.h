#pragma once

#ifdef _DEBUG
#   if defined(_WIN32) || defined(_WIN64)
#       pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#   endif
#endif

//미리 컴파일된 헤더.
//pch pre compiled header
#include "Platform/Platform_Base.h"

// C, C++ 헤더입니다.
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <time.h>
#include <chrono>
#include <stdlib.h>
#include <string>
#include <map>
#include <unordered_map>
#include <cassert>
#include <random>
#include <functional>
#include <fstream>
#include <ctime>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <future>
#include <sstream>
#include <exception>
#include "Utilities/ServiceLocator.h"
#include "Utilities/ServiceRegister.h"

using namespace std;

#define WIN_DEFAULT_WIDTH 1280
//#define WIN_DEFAULT_WIDTH 1920
//#define WIN_DEFAULT_WIDTH 2560
#define WIN_DEFAULT_HEIGHT 720
//#define WIN_DEFAULT_HEIGHT 1080
//#define WIN_DEFAULT_HEIGHT 1440

#define CENTER_X (WIN_DEFAULT_WIDTH / 2)
#define CENTER_X_2_1 (WIN_DEFAULT_WIDTH / 2.1f)
#define CENTER_X_2_2 (WIN_DEFAULT_WIDTH / 2.2f)
#define CENTER_X_2_3 (WIN_DEFAULT_WIDTH / 2.3f)
#define CENTER_X_2_5 (WIN_DEFAULT_WIDTH / 2.5f)
#define CENTER_X_3 (WIN_DEFAULT_WIDTH / 3.0f)
#define CENTER_Y (WIN_DEFAULT_HEIGHT / 2)
#define CENTER_Y_2_1 (WIN_DEFAULT_HEIGHT / 2.1f)
#define CENTER_Y_2_2 (WIN_DEFAULT_HEIGHT / 2.2f)
#define CENTER_Y_2_3 (WIN_DEFAULT_HEIGHT / 2.3f)
#define CENTER_Y_2_5 (WIN_DEFAULT_HEIGHT / 2.5f)
#define CENTER_Y_3 (WIN_DEFAULT_HEIGHT / 3.0f)
#define CENTER (Vector3(CENTER_X, CENTER_Y, 0))

#define CHECK(hr) { assert(SUCCEEDED(hr)); }

#define BASE __super

#define SAFE_DELETE(p) { if (p) {delete p ; p = nullptr;} }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p ; p = nullptr;} }
#define SAFE_RELEASE(p)		 { if(p) { (p)->Release(); (p) = nullptr; } }   

#define DECLARE_SINGLETON(CLASS_NAME)						\
private:													\
CLASS_NAME();												\
~CLASS_NAME();												\
public:														\
CLASS_NAME(const CLASS_NAME& other) = delete;				\
CLASS_NAME& operator=(const CLASS_NAME& other) = delete;	\
CLASS_NAME(const CLASS_NAME&& other) = delete;				\
CLASS_NAME& operator=(const CLASS_NAME&& other) = delete;	\
public:														\
static CLASS_NAME* Get()									\
{															\
	static CLASS_NAME instance;								\
	return &instance;										\
}

#define DECLARE_SINGLETON_DYNAMIC(CLASS_NAME)                   \
private:                                                        \
    static CLASS_NAME* instance;                                \
    CLASS_NAME();                                               \
    ~CLASS_NAME();                                              \
public:                                                         \
    CLASS_NAME(const CLASS_NAME& other) = delete;               \
    CLASS_NAME& operator=(const CLASS_NAME& other) = delete;    \
    CLASS_NAME(const CLASS_NAME&& other) = delete;              \
    CLASS_NAME& operator=(const CLASS_NAME&& other) = delete;   \
public:                                                         \
    static CLASS_NAME* Create()                                 \
    {                                                           \
        if (!instance) instance = new CLASS_NAME();             \
        return instance;                                        \
    }                                                           \
    static CLASS_NAME* Get()                                    \
    {                                                           \
        if (!instance)                                          \
            instance = new CLASS_NAME();                        \
        return instance;                                        \
    }                                                           \
    static void Delete()                                        \
    {                                                           \
        delete instance;                                        \
        instance = nullptr;                                     \
    }

//ExternGlobals
extern float gWinWidth;
extern float gWinHeight;
extern Platform::WindowHandle gHandle;
#if defined(_WIN32) || defined(_WIN64)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

//Singleton accessors via ServiceLocator
#define INPUT ServiceLocator::Get<Input>("Input").get()
#define TIME ServiceLocator::Get<Time>("Time").get()
#define DELTA ServiceLocator::Get<Time>("Time")->GetDeltaTime()
#define GRAPHICS ServiceLocator::Get<Graphics>("Graphics").get()
#define DEVICE ServiceLocator::Get<Graphics>("Graphics")->GetDevice()
#define DC ServiceLocator::Get<Graphics>("Graphics")->GetDC()
#define SWAPCHAIN ServiceLocator::Get<Graphics>("Graphics")->GetSwapChain()
#define D2DC ServiceLocator::Get<Graphics>("Graphics")->GetD2DC()
#define WRITE ServiceLocator::Get<Graphics>("Graphics")->GetWrite()
#define DEBUG ServiceLocator::Get<HelpMe>("HelpMe").get()
#define CAMERA ServiceLocator::Get<Camera>("Camera").get()
#define SCENE ServiceLocator::Get<SceneManager>("SceneManager").get()
#define SOUNDS ServiceLocator::Get<Sounds>("Sounds").get()
#define IMGUI ServiceLocator::Get<Gui>("Gui").get()
#define KEYBOARD ServiceLocator::Get<Keyboard>("Keyboard").get()
#define MOUSE ServiceLocator::Get<Mouse>("Mouse").get()
#define COLLISION ServiceLocator::Get<CollisionManager>("CollisionManager").get()
#define GIZMO ServiceLocator::Get<Gizmo>("Gizmo").get()
#define REALDELTA ServiceLocator::Get<Time>("Time")->GetRealDeltaTime()
#define TILESET ServiceLocator::Get<TileSetManager>("TileSetManager").get()
#define TEXTURE2D ServiceLocator::Get<Texture2DManager>("Texture2DManager").get()

//ColorMacros
#define RED Color(1, 0, 0, 1)
#define GREEN Color(0, 1, 0, 1)
#define BLUE Color(0, 0, 1, 1)
#define YELLOW Color(1, 1, 0, 1)
#define MAGENTA Color(1, 0, 1, 1)
#define CYAN Color(0, 1, 1, 1)
#define WHITE Color(1, 1, 1, 1)
#define BLACK Color(0, 0, 0, 1)
#define CLEAR Color(1, 1, 1, 0)
#define GRAY Color(0.4f, 0.4f, 0.4f, 1.0f)
#define CLEAR_COLOR Color(0xcff0cc);

const wstring ShaderPath = L"_Data/_Shaders/";
const wstring TexturePath = L"_Data/_Textures/";
const wstring SoundPath = L"_Data/_Sounds/";
const wstring FontPath = L"_Data/_Fonts/";
const string TilePath = "../../../_Data/_TileData/";
const string BuildTilePath = "_Data/_TileData/";

// Managers
#include "Systems/Camera.h"
#include "Systems/Graphics.h"
#include "Systems/Input.h"
#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Sounds.h"
#include "Systems/Time.h"
#include "Systems/Window.h"

#include "Utilities/DirectHelper.h"
#include "Utilities/FileStream.h"
#include "Utilities/HelpMe.h"
#include "Utilities/Path.h"
#include "Utilities/Random.h"
#include "Utilities/String.h"


