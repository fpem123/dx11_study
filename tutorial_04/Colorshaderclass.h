/*
 * Colorshaderclass.h : Colorshaderclass.cpp를 위한 header
 * 작성일 : 10.01.21
 * 마지막 업데이트 : 10.01.21
 */
#pragma once

#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

////////////////
// includes
////////////////
#include "DxDefine.h"
#include <fstream>
using namespace std;


////////////////
// ColorShaderClass
////////////////
class ColorShaderClass
{
private:
	// 셰이더에 사용될 cBuffer 구조체
	// 셰이더가 올바른 렌더링을 하기 위해서는 
	// 이 typedef 문의 구조체가 반드시
	// 정점 셰이더에 사용된 구조체와 동일해야함
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	// Render : 셰이더에 사용되는 변수들을 설정하고
	// 셰이더를 이용해 준비된 모델의 정점을 그려냄
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif // !_COLORSHADERCLASS_H_
