#pragma once
/*
 * Graphicsclass.h : Graphicsclass를 위한 헤더
 * D3Dclass의 가장 좋은 위치
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 09.27.21
 */
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// includes
// windows.h 대신 D3Dclass를 include
#include "d3dclass.h"

// globlas
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1;

// GraphicsClass
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	// 새로운 D3DClass 포인터
	D3DClass* m_D3D;
};


#endif // !_GRAPHICSCLASS_H_
