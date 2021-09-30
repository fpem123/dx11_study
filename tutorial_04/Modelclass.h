/*
 * Modelclass.h : Modelclass.cpp를 위한 header
 * 작성일 : 09.30.21
 * 마지막 업데이트 : 09.30.21
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
	// 정점 버퍼에 사용할 정점의 구조체를 선언
	// ColorShaderClass에서 사용할 것과 구조가 같아야함
	struct VertextType
	{
		// 레거시 D3DXVECTOR 는 XMFLOAT 로 변경됨
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// 3D 모델의 정점 버퍼와 인덱스 버퍼들의 초기화, 종료 과정을 제어
	bool Initialize(ID3D11Device*);
	void Shutdown();
	// Render 함수는 GPU에 모델 기하 정보를 넣고
	// 컬러 셰이더로 그릴 준비를 함
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	// 정점 버버퐈 인덱스 버퍼
	// 버퍼들은 대개 ID3D11Buffer 타입을 이용
	// 생성할 때의 버퍼 description 으로 구분
	ID3D11Buffer* m_vertextBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	// 각 버퍼의 크기 정보
	int m_vertextCount = 0;
	int m_indexCount = 0;
};

#endif // !_MODELCLASS_H_
