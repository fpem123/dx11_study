/*
 * Cameraclass.cpp : 우리가 월드를 보는 시점
 * 어떻게 우리가 장면을 보는지에 대한 정보를 DirectX 에게 전달
 * 카메라 위치와 회전 상태를 계속 가지고 있어야함
 * 이 정보를 이용해 렌더링시 HLSL 셰이더에 사용할 뷰 행렬을 생성
 * 작성일 : 10.01.21
 * 마지막 업데이트 : 10.02.21
 */

///////////////
// include
///////////////
#include "Cameraclass.h"


CameraClass::CameraClass()
{
	// 카메라 위치와 회전  상태를 원점으로 설정
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}


// 카메라의 위치를 정하는 데 사용
void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	return;
}


// 카메라의 회전을 정하는 데 사용
void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;

	return;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}


// 카메라의 위치와 회전 정보를 이용해 뷰 행렬을 갱신
void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// 위를 가리키는 벡터 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR 구조체에 로드
	upVector = XMLoadFloat3(&up);

	// 월드의 카메라 위치 설정
	position = m_position;

	// XMVECTOR 구조체에 로드
	positionVector = XMLoadFloat3(&position);

	// 카메라가 기본적으로 바라보는 위치 설정
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	
	// XMVECTOR 구조체에 로드
	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw(y축), pitch(x축), roll(z축) 회전 정보를 라디안값으로 설정
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// yaw, pitch, roll로 회전 행렬을 만듬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 시점이 원점에서 올바르게 회전되도록 회전행렬로 
	// lootAt과 up 벡터를 변환
	// D3DXVec3TransformCoord -> XMVector3TransformCoord
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// 회전된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);


	// 세계의 업데이트 된 벡터에서 뷰 행렬을 만듬
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


// 뷰 행렬을 생성하는 Render 함수 수행 후 이 함수로 뷰 행렬을 얻을 수 있음
// 뷰 행렬은 HLSL 정점 셰이더에서 사용하는 중요한 세 행렬 중 하나
void CameraClass::GetviewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
