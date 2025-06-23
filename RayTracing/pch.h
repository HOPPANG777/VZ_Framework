// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

// 디버그 모드일 때
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") // 콘솔창을 띄어줌
#endif // #if나 #ifdef를 했다면 endif로 끝내야함

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h> // lParam으로 부터 x, y를 분리 하기 위한 함수가 담긴 헤더 파일만 추가해줌 (위 define으로 사용 불가됨
// C 런타임 헤더 파일입니다.
#include <cassert>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

//D3D
#include <d3d11.h>
#include <d3dcompiler.h>

//WRL(for ComPtr) // 스마트 포인트 같은 애며, 실제로 스마트 포인터 처럼 사용함
#include <wrl.h>
using namespace Microsoft;
using namespace WRL;

//DirectXTK
#include <directxtk/SimpleMath.h> // <>를 이용해서 폴더 찾아가서 사용 가능.
using namespace DirectX;
using namespace SimpleMath;

//IMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

using namespace std;

//ReadData
#include "ReadData.h"

//Macro
#define CHECK(hr) { if (FAILED(hr)) std::cout << "Error Code : 0x" << std::hex << hr << " - " << std::system_category().message(hr) << "\n"; assert(SUCCEEDED(hr)); }

#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] p; p = nullptr; } }
