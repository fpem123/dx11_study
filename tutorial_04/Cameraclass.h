/*
 * Cameraclass.h : Cameraclass.cpp를 위한 header
 * 우리가 월드를 보는 시점
 * 작성일 : 10.01.21
 * 마지막 업데이트 : 10.01.21
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

