#pragma once
/*
 * Graphicsclass.h : Graphicsclass를 위한 헤더
 * D3Dclass의 가장 좋은 위치
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 10.01.21
 */
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////
// includes
///////////////
// 세게의 새로운 클래스 추가
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
	// 새로운 D3DClass 포인터
	D3DClass* m_D3D;
	// 새로 추가된 클래스들에 대한 멤버 변수
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};

#endif // !_GRAPHICSCLASS_H_

