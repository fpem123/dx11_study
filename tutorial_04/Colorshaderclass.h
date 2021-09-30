/*
 * Colorshaderclass.h : Colorshaderclass.cpp�� ���� header
 * �ۼ��� : 10.01.21
 * ������ ������Ʈ : 10.01.21
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
	// ���̴��� ���� cBuffer ����ü
	// ���̴��� �ùٸ� �������� �ϱ� ���ؼ��� 
	// �� typedef ���� ����ü�� �ݵ��
	// ���� ���̴��� ���� ����ü�� �����ؾ���
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
	// Render : ���̴��� ���Ǵ� �������� �����ϰ�
	// ���̴��� �̿��� �غ�� ���� ������ �׷���
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, HWND, WCHAR*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif // !_COLORSHADERCLASS_H_