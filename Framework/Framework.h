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
#include <mutex>
#include <thread>
#include <unordered_set>
#include <future>
#include <sstream>
#include <exception>

using namespace std;

//////////////////////////////////////////
//DirectX D3D11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <D3DX11tex.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
//////////////////////////////////////////

//////////////////////////////////////////
//Direct 2D, Direct Write
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <dwrite.h>
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
using namespace D2D1;
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
extern HWND gHandle;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//SingletonMacros
#define INPUT Input::Get()
#define TIME Time::Get()
#define DELTA Time::Get()->GetDeltaTime()
#define GRAPHICS Graphics::Get()
#define DEVICE GRAPHICS->GetDevice()
#define DC GRAPHICS->GetDC()
#define SWAPCHAIN GRAPHICS->GetSwapChain()
#define D2DC GRAPHICS->GetD2DC()
#define WRITE GRAPHICS->GetWrite()	
#define FONT Texts::Get()
#define DEBUG HelpMe::Get()
#define CAMERA Camera::Get()
#define SCENE SceneManager::Get()
#define SOUNDS Sounds::Get()
#define IMGUI Gui::Get()
#define KEYBOARD Keyboard::Get()
#define MOUSE Mouse::Get()
#define COLLISION CollisionManager::Get()
#define GIZMO Gizmo::Get()
#define REALDELTA TIME->GetRealDeltaTime()
#define TILESET TileSetManager::Get()
#define TEXTURE2D Texture2DManager::Get()
#define INPUT Input::Get()

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

//Interface
#include "Interfaces/IObject.h"
#include "Interfaces/IScene.h"
#include "Interfaces/IColorable.h"

//Struct
#include "Structs/FontBrushDesc.h"
#include "Structs/FontTextDesc.h"
#include "Structs/TextureDesc.h"
#include "Structs/WinDesc.h"

//Enum
#include "Enums/UnitAnimationType.h"

//Systems
#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Systems/Graphics.h"

#include "Utilities/Gui.h"

//Utilities
#include "Utilities/DirectHelper.h"
#include "Utilities/HelpMe.h"
#include "Utilities/Random.h"
#include "Utilities/String.h"
#include "Utilities/Path.h"
#include "Utilities/FileStream.h"
#include "Utilities/Gizmo.h"
#include "Utilities/AnimationClip.h"

//Renders
#include "Renders/RenderResources/ConstantBuffer.h"
#include "Renders/RenderResources/GlobalBuffers.h"
#include "Renders/RenderResources/VertexType.h"
#include "Renders/RenderResources/Texture2D.h"

#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/IA/VertexBuffer.h"
#include "Renders/RenderStates.h"

#include "Renders/Shaders/IShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Renders/Shaders/VertexShader.h"

//Components
#include "Components/Component.h"
#include "Components/MoveComponent.h"
#include "Components/Collider.h"
#include "Components/BoxCollider.h"
#include "Components/CircleCollider.h"
#include "Components/Texts.h"
#include "Components/TypingText.h"
#include "Components/ShakingText.h"
#include "Components/Transform.h"
#include "Components/Animator.h"
#include "Components/RectTransform.h"
#include "Components/Button.h"
#include "Components/AnimationMove.h"
#include "Components/PlayerMove.h"

//Object
#include "Objects/BaseObject/Object.h"
#include "Objects/BaseObject/Drawable.h"
#include "Objects/ColorObject/ColorRect.h"
#include "Objects/ColorObject/ColorCircle.h"
#include "Objects/ColorObject/ColorFilledCircle.h"
#include "Objects/ColorObject/ColorLine.h"
#include "Objects/TextureObject/TextureRect.h"
#include "Objects/TextureObject/AnimationRect.h"
#include "Objects/TileObject/Tile.h"
#include "Objects/SpriteObject/Sprite.h"
#include "Objects/TileObject/TileMap.h"

//Camera
#include "Systems/Camera.h"

//Sounds
#include "Systems/Sounds.h"

//Managers
#include "Managers/SceneManager.h"
#include "Managers/CollisionManager.h"
#include "Managers/TileSetManager.h"
#include "Managers/Texture2DManager.h"

// Editor
