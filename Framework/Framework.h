#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

//미리 컴파일된 헤더.
//pch pre compiled header

#include "Utilities/targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// C, C++ 헤더입니다.
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <time.h>
#include <chrono>
#include <stdlib.h>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <map>
#include <unordered_map>
#include <cassert>
#include <random>
#include <functional>
#include <fstream>
#include <windowsx.h>
#include <ctime>

using namespace std;

//////////////////////////////////////////
//DirectX D3D11
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
//////////////////////////////////////////

//////////////////////////////////////////
//WRL
#include <wrl.h>
using namespace Microsoft;
using namespace WRL;
//////////////////////////////////////////

//////////////////////////////////////////
//DirectXTK
#include <directxtk/SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
//////////////////////////////////////////

//////////////////////////////////////////
// DirectXTex
#include <DirectXTex.h>
//////////////////////////////////////////

//////////////////////////////////////////
//IMGUI
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
//////////////////////////////////////////

//////////////////////////////////////////
// FMOD
#include "FMOD/inc/fmod.hpp"
#ifdef _WIN64
#pragma comment(lib, "FMOD/lib/x64/fmod_vc.lib")
#else
#pragma comment(lib, "FMOD/lib/x86/fmod_vc.lib")
#endif
//////////////////////////////////////////

#define WIN_DEFAULT_WIDTH 1280
#define WIN_DEFAULT_HEIGHT 720

#define CENTER_X (WIN_DEFAULT_WIDTH / 2)
#define CENTER_Y (WIN_DEFAULT_HEIGHT / 2)
#define CENTER (Vector2(CENTER_X, CENTER_Y))

#define CHECK(hr) { assert(SUCCEEDED(hr)); }

#define BASE __super

#define SAFE_DELETE(p) { if (p) {delete p ; p = nullptr;} }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p ; p = nullptr;} }

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

//ExternGlobals
extern float gWinWidth;
extern float gWinHeight;
extern HWND gHandle;

//SingletonMacros
#define INPUT Input::Get()
#define TIME Time::Get()
#define DELTA Time::Get()->GetDeltaTime()
#define GRAPHICS Graphics::Get()
#define DEVICE GRAPHICS->GetDevice()
#define DC GRAPHICS->GetDC()
#define DEBUG HelpMe::Get()
#define CAMERA Camera::Get()
#define SCENE SceneManager::Get()
#define SOUNDS Sounds::Get()
#define IMGUI Gui::Get()
#define KEYBOARD Keyboard::Get()
#define MOUSE Mouse::Get()


//ColorMacros
#define RED Color(1, 0, 0, 1)
#define GREEN Color(0, 1, 0, 1)
#define BLUE Color(0, 0, 1, 1)
#define YELLOW Color(1, 1, 0, 1)
#define MAGENTA Color(1, 0, 1, 1)
#define CYAN Color(0, 1, 1, 1)
#define WHITE Color(1, 1, 1, 1)
#define BLACK Color(0, 0, 0, 1)

//Interface
#include "Interfaces/IObject.h"

//Systems
#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Systems/Graphics.h"

#include "Utilities/Gui.h"

//Utilities
#include "Utilities/Animator.h"
#include "Utilities/DirectHelper.h"
#include "Utilities/HelpMe.h"
#include "Utilities/Random.h"
#include "Utilities/SceneManager.h"
#include "Utilities/String.h"
#include "Utilities/Path.h"
#include "Utilities/FileStream.h"

//Renders
#include "Renders/Resources/ConstantBuffer.h"
#include "Renders/Resources/GlobalBuffers.h"
#include "Renders/Resources/VertexType.h"
#include "Renders/Resources/Texture2D.h"

#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/IA/VertexBuffer.h"
#include "Renders/RenderStates.h"

#include "Renders/Shaders/IShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Renders/Shaders/VertexShader.h"

//Components
#include "Components/Component.h"
#include "Components/GameComponent.h"
#include "Components/MoveComponent.h"

//Object
#include "Objects/BaseObject/Object.h"
#include "Objects/BaseObject/GameObject.h"
#include "Objects/BaseObject/Drawable.h"

//Camera
#include "Systems/Camera.h"

//Sounds
#include "Systems/Sounds.h"

const wstring ShaderPath = L"../_Shaders/";
const wstring TexturePath = L"../_Textures/";
const wstring SoundPath = L"../_Sounds/";
const string TilePath = "../_TileData/";
