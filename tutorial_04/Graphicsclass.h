#pragma once
/*
 * Graphicsclass.h : Graphicsclass�� ���� ���
 * D3Dclass�� ���� ���� ��ġ
 * �ۼ��� : 09.26.21
 * ������ ������Ʈ : 10.01.21
 */
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////
// includes
///////////////
// ������ ���ο� Ŭ���� �߰�
#include "d3dclass.h"
#include "Cameraclass.h"
#include "Modelclass.h"
#include "Colorshaderclass.h"

///////////////
// globlas
///////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1;

///////////////
// GraphicsClass
///////////////
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
	// ���� �߰��� Ŭ�����鿡 ���� ��� ����
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};

#endif // !_GRAPHICSCLASS_H_

