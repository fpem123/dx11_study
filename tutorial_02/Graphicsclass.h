#pragma once
/*
 * Graphicsclass.h : Graphicsclass를 위한 헤더
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 09.26.21
 */
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// includes
#include <windows.h>

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

};


#endif // !_GRAPHICSCLASS_H_
