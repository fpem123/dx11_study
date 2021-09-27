#pragma once
/*
 * Graphicsclass.h : Graphicsclass�� ���� ���
 * D3Dclass�� ���� ���� ��ġ
 * �ۼ��� : 09.26.21
 * ������ ������Ʈ : 09.27.21
 */
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// includes
// windows.h ��� D3Dclass�� include
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
	// ���ο� D3DClass ������
	D3DClass* m_D3D;
};


#endif // !_GRAPHICSCLASS_H_
