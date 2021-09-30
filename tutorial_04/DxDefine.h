#pragma once
/*
 * DxDefine.h : directX sdk를 사용하기위한
 * 헤더와 라이브러리 코드들이 위치함
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 10.01.21
 */

/////////////////
// linking
/////////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/////////////////
// includes
/////////////////
// 링크한 라이브러리들을 include
#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;