#pragma once


//#include "Utilities/targetver.h"
// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>
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