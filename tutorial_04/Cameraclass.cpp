/*
 * Cameraclass.cpp : �츮�� ���带 ���� ����
 * ��� �츮�� ����� �������� ���� ������ DirectX ���� ����
 * ī�޶� ��ġ�� ȸ�� ���¸� ��� ������ �־����
 * �� ������ �̿��� �������� HLSL ���̴��� ����� �� ����� ����
 * �ۼ��� : 10.01.21
 * ������ ������Ʈ : 10.02.21
 */

///////////////
// include
///////////////
#include "Cameraclass.h"


CameraClass::CameraClass()
{
	// ī�޶� ��ġ�� ȸ��  ���¸� �������� ����
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}


// ī�޶��� ��ġ�� ���ϴ� �� ���
void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	return;
}


// ī�޶��� ȸ���� ���ϴ� �� ���
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


// ī�޶��� ��ġ�� ȸ�� ������ �̿��� �� ����� ����
void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// ���� ����Ű�� ���� ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR ����ü�� �ε�
	upVector = XMLoadFloat3(&up);

	// ������ ī�޶� ��ġ ����
	position = m_position;

	// XMVECTOR ����ü�� �ε�
	positionVector = XMLoadFloat3(&position);

	// ī�޶� �⺻������ �ٶ󺸴� ��ġ ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	
	// XMVECTOR ����ü�� �ε�
	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw(y��), pitch(x��), roll(z��) ȸ�� ������ ���Ȱ����� ����
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// yaw, pitch, roll�� ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// ������ �������� �ùٸ��� ȸ���ǵ��� ȸ����ķ� 
	// lootAt�� up ���͸� ��ȯ
	// D3DXVec3TransformCoord -> XMVector3TransformCoord
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// ȸ���� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);


	// ������ ������Ʈ �� ���Ϳ��� �� ����� ����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


// �� ����� �����ϴ� Render �Լ� ���� �� �� �Լ��� �� ����� ���� �� ����
// �� ����� HLSL ���� ���̴����� ����ϴ� �߿��� �� ��� �� �ϳ�
void CameraClass::GetviewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
