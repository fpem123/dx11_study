/*
 * Modelclass.cpp : 3D �𵨵��� ������ �����е��� ĸ��ȭ
 * ��� �ﰢ���� ����� ���� �����͸� ����
 * �ﰢ���� �׸��� ���� ���� ���ۿ� �ε��� ���� ����
 * �ۼ��� : 09.30.21
 * ������ ������Ʈ : 09.30.21
 */

/////////////////
// incldues
/////////////////
#include "Modelclass.h"

// �����ڿ��� ���� ���ۿ� �ε��� ������ �����͸� null�� ����
ModelClass::ModelClass()
{
	m_vertextBuffer = 0;
	m_indexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

// ���� ���ۿ� �ε��� ������ �ʱ�ȭ �Լ��� ȣ��
bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	// ���� ���ۿ� �ε��� ���� �ʱ�ȭ
	result = InitializeBuffers(device);

	if (!result)
	{
		return false;
	}

	return true;
}

// ���� ���ۿ� �ε��� ���۸� ����
void ModelClass::Shutdown()
{
	// ���� ���ۿ� �ε��� ���۸� ����
	ShutdownBuffers();

	return;
}

// GraphicsClass::Render �Լ����� ȣ��
// RenderBuffers �Լ��� ȣ���Ͽ� ���� ���ۿ� �ε��� ���۸�
// �׷������������ο� �־� �÷� ���̴��� �׸� �� �ֵ��� ��
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// ���� ���ۿ� �ε��� ���۸� �׷��Ƚ� ���������ο� �־�
	// ȭ�鿡 �׸� �غ� ��
	RenderBuffers(deviceContext);

	return;
}

// �ش� ���� �ε��� ���� �˷���
// �÷� ���̴����� ���� �׸��� ���ؼ� �� ������ �ʿ�
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// ���� ���ۿ� �ε��� ���۸� �����ϴ� �۾��� ����
// ���� ������ ���Ϸ� ���� ���� ������ �о�ͼ� ���۸� ����
// ���⼱ ������ ���� ���ۿ� �ε��� ���ۿ� ���� ����
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertextType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertextBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertextData, indexData;
	HRESULT result;

	// ������ �ε��� �����͸� ���� �� �ӽ� �迭�� �����
	// ���߿� ���� ���۸� ������ �� ���

	// ���� �迭�� ���̸� ����
	m_vertextCount = 6;

	// �ε��� �迭�� ���̸� ����
	m_indexCount = 6;

	// ���� �迭�� ����
	vertices = new VertextType[m_vertextCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ����, �ε��� �迭�� �ﰢ�� ���� ������ ä��
	// ����) �̰͵��� �׸��� ���ؼ� ������ �ð� �������� ��������
	// �ݽð�� ����� DirextX���� �ﰢ���� �ݴ����� �ٶ󺻴� �Ǵ�
	// backface culling �� ���� �׷����� �ʰ� ��
	// GPU���� ������ �׸����� �� �� �� ������ ����ϴ� ���� �߿�

	// ���� �迭�� ���� ����
	// ���� �Ʒ�
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// ��� ���
	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// ������ �Ʒ�
	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// 2�� ° �ﰢ��
	// ���� �Ʒ�
	vertices[3].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// ��� ���
	vertices[4].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[4].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// ������ �Ʒ�
	vertices[5].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// �ε��� �迭�� ���� ����
	indices[0] = 0;		// ���� �Ʒ�
	indices[1] = 1;		// ��� ���
	indices[2] = 2;		// ������ �Ʒ�


	indices[3] = 3;		// ���� �Ʒ�
	indices[4] = 4;		// ��� ���
	indices[5] = 5;		// ������ �Ʒ�


	// ����, �ε��� ���۸� ����
	// �� ���۸� ����� ���� �����


	// ���� ������ description�� �ۼ�
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// ByteWidth, BindFlags�� ��Ȯ�� �Է��ؾ���
	vertextBufferDesc.ByteWidth = sizeof(VertextType) * m_vertextCount;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.MiscFlags = 0;
	vertextBufferDesc.StructureByteStride = 0;

	// ���� �����͸� ����Ű�� Sub resorce ����ü �ۼ�
	vertextData.pSysMem = vertices;
	vertextData.SysMemPitch = 0;
	vertextData.SysMemSlicePitch = 0;

	// ���� ���۸� ����
	result = device->CreateBuffer(&vertextBufferDesc,
		&vertextData, &m_vertextBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ε��� ������ description�� �ۼ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� sub resource ����ü �ۼ�
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����
	result = device->CreateBuffer(&indexBufferDesc,
		&indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// �̹� ���� ����Ǿ� �ʿ� ������ ���� �迭�� �ε��� �迭�� ����


	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� ����
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


// InitializeBuffers �Լ����� �������
// ���� ���ۿ� �ε��� ���۸� ����
void ModelClass::ShutdownBuffers()
{
	// �ε��� ���� ����
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� ����
	if (m_vertextBuffer)
	{
		m_vertextBuffer->Release();
		m_vertextBuffer = 0;
	}

	return;
}


// Render �Լ����� ȣ��
// ���� ���ۿ� �ε��� ���۸� GPU ������� ���۷� Ȱ��ȭ ��Ŵ
// GPU�� Ȱ��ȭ�� ���� ���۸� ������ ���̴��� �̿��� ������ �׸�
// ������ � ������� �׸��� �� �� ����
// ������� �Է¿� ���� ���ۿ� �ε��� ���۸� �ְ�
// DirectX�� IASetPrimitiveTopology �Լ��� ����� 
// �ﰢ���� �׸����� �ֹ�
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// ���� ������ ������ �������� ����
	stride = sizeof(VertextType);
	offset = 0;

	// input assembler�� ���� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ� ��
	deviceContext->IASetVertexBuffers(0, 1, &m_vertextBuffer,
		&stride, &offset);

	// input assembler�� �ε��� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ���
	deviceContext->IASetIndexBuffer(m_indexBuffer,
		DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� ����, �ﰢ��
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}