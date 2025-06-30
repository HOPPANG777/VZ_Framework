#pragma once

#include "Utilities/targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

//////////////////////////////////////////
//Windows Platform
#include <windows.h>
#include <commdlg.h>
#include <windowsx.h>
#include <crtdbg.h>
//////////////////////////////////////////

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
