/*
 * Cameraclass.h : Cameraclass.cpp�� ���� header
 * �츮�� ���带 ���� ����
 * �ۼ��� : 10.01.21
 * ������ ������Ʈ : 10.01.21
 */
#pragma once
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

/////////////
// includes
/////////////
#include "DxDefine.h"

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetviewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};

#endif // !_CAMERACLASS_H_

