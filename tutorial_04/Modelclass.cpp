/*
 * Modelclass.cpp : 3D 모델들의 복잡한 기하학들을 캡슐화
 * 녹색 삼각형을 만들기 위한 데이터를 만듬
 * 삼각형을 그리기 위한 정점 버퍼와 인덱스 버퍼 포함
 * 작성일 : 09.30.21
 * 마지막 업데이트 : 09.30.21
 */

/////////////////
// incldues
/////////////////
#include "Modelclass.h"

// 생성자에서 정점 버퍼와 인덱스 버퍼의 포인터를 null로 설정
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

// 정점 버퍼와 인덱스 버퍼의 초기화 함수를 호출
bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	// 정점 버퍼와 인덱스 버퍼 초기화
	result = InitializeBuffers(device);

	if (!result)
	{
		return false;
	}

	return true;
}

// 정점 버퍼와 인덱스 버퍼를 정리
void ModelClass::Shutdown()
{
	// 정점 버퍼와 인덱스 버퍼를 해제
	ShutdownBuffers();

	return;
}

// GraphicsClass::Render 함수에서 호출
// RenderBuffers 함수를 호출하여 정점 버퍼와 인덱스 버퍼를
// 그래픽파이프라인에 넣어 컬러 셰이더가 그릴 수 있도록 함
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어
	// 화면에 그릴 준비를 함
	RenderBuffers(deviceContext);

	return;
}

// 해당 모델의 인덱스 수를 알려줌
// 컬러 셰이더에서 모델을 그리기 위해선 이 정보가 필요
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// 정점 버퍼와 인덱스 버퍼를 생성하는 작업을 제어
// 보통 데이터 파일로 부터 모델의 정보를 읽어와서 버퍼를 만듬
// 여기선 간단히 정점 버퍼와 인덱스 버퍼에 점을 세팅
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertextType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertextBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertextData, indexData;
	HRESULT result;

	// 정점과 인덱스 데이터를 담을 두 임시 배열을 만들고
	// 나중에 최종 버퍼를 생성할 때 사용

	// 정점 배열의 길이를 설정
	m_vertextCount = 6;

	// 인덱스 배열의 길이를 설정
	m_indexCount = 6;

	// 정점 배열을 생성
	vertices = new VertextType[m_vertextCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 생성
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점, 인덱스 배열에 삼각형 점과 순서를 채움
	// 주의) 이것들을 그리기 위해선 점들을 시계 방향으로 만들어야함
	// 반시계로 만들면 DirextX에서 삼각형이 반대편을 바라본다 판단
	// backface culling 에 의해 그려지지 않게 됨
	// GPU에게 도형을 그리도록 할 때 이 순서를 기억하는 것이 중요

	// 정점 배열에 값을 넣음
	// 왼쪽 아래
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// 상단 가운데
	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// 오른쪽 아래
	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// 2번 째 삼각형
	// 왼쪽 아래
	vertices[3].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// 상단 가운데
	vertices[4].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[4].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// 오른쪽 아래
	vertices[5].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[5].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// 인덱스 배열에 값을 넣음
	indices[0] = 0;		// 왼쪽 아래
	indices[1] = 1;		// 상단 가운데
	indices[2] = 2;		// 오른쪽 아래


	indices[3] = 3;		// 왼쪽 아래
	indices[4] = 4;		// 상단 가운데
	indices[5] = 5;		// 오른쪽 아래


	// 정점, 인덱스 버퍼를 만듬
	// 두 버퍼를 만드는 일은 비슷함


	// 정점 버퍼의 description을 작성
	vertextBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// ByteWidth, BindFlags을 정확히 입력해야함
	vertextBufferDesc.ByteWidth = sizeof(VertextType) * m_vertextCount;
	vertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertextBufferDesc.CPUAccessFlags = 0;
	vertextBufferDesc.MiscFlags = 0;
	vertextBufferDesc.StructureByteStride = 0;

	// 정점 데이터를 가리키는 Sub resorce 구조체 작성
	vertextData.pSysMem = vertices;
	vertextData.SysMemPitch = 0;
	vertextData.SysMemSlicePitch = 0;

	// 정점 버퍼를 생성
	result = device->CreateBuffer(&vertextBufferDesc,
		&vertextData, &m_vertextBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 인덱스 버퍼의 description을 작성
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 sub resource 구조체 작성
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
	result = device->CreateBuffer(&indexBufferDesc,
		&indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// 이미 값이 복사되어 필요 없어진 정점 배열과 인덱스 배열을 제거


	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


// InitializeBuffers 함수에서 만들었던
// 정점 버퍼와 인덱스 버퍼를 해제
void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼 해제
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제
	if (m_vertextBuffer)
	{
		m_vertextBuffer->Release();
		m_vertextBuffer = 0;
	}

	return;
}


// Render 함수에서 호출
// 정점 버퍼와 인덱스 버퍼를 GPU 어셈블러의 버퍼로 활성화 시킴
// GPU가 활성화된 정점 버퍼를 가지면 셰이더를 이용해 내용을 그림
// 정점이 어떤 모양으로 그리게 될 지 정의
// 어셈블러의 입력에 정점 버퍼와 인덱스 버퍼를 넣고
// DirectX의 IASetPrimitiveTopology 함수를 사용해 
// 삼각형을 그리도록 주문
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 정점 버퍼의 단위와 오프셋을 설정
	stride = sizeof(VertextType);
	offset = 0;

	// input assembler에 정점 버퍼를 활성화하여 그려질 수 있게 함
	deviceContext->IASetVertexBuffers(0, 1, &m_vertextBuffer,
		&stride, &offset);

	// input assembler에 인덱스 버퍼를 활성화하여 그려질 수 있게함
	deviceContext->IASetIndexBuffer(m_indexBuffer,
		DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정, 삼각형
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}