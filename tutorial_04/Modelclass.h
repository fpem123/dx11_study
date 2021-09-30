/*
 * Modelclass.h : Modelclass.cpp�� ���� header
 * �ۼ��� : 09.30.21
 * ������ ������Ʈ : 09.30.21
 */

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

/////////////////
// includes
/////////////////
#include "DxDefine.h"

/////////////////
// ModelClass
/////////////////
class ModelClass
{
private:
	// ���� ���ۿ� ����� ������ ����ü�� ����
	// ColorShaderClass���� ����� �Ͱ� ������ ���ƾ���
	struct VertextType
	{
		// ���Ž� D3DXVECTOR �� XMFLOAT �� �����
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// 3D ���� ���� ���ۿ� �ε��� ���۵��� �ʱ�ȭ, ���� ������ ����
	bool Initialize(ID3D11Device*);
	void Shutdown();
	// Render �Լ��� GPU�� �� ���� ������ �ְ�
	// �÷� ���̴��� �׸� �غ� ��
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	// ���� ����ǡ �ε��� ����
	// ���۵��� �밳 ID3D11Buffer Ÿ���� �̿�
	// ������ ���� ���� description ���� ����
	ID3D11Buffer* m_vertextBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	// �� ������ ũ�� ����
	int m_vertextCount = 0;
	int m_indexCount = 0;
};

#endif // !_MODELCLASS_H_
