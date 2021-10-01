/*
 * GraphicsClass.cpp : SystemClass가 생성하는 또 다른 객체
 * app의 모든 그래픽 기능이 캡술화 되어있음
 * D3DClass를 멤버로 가지고
 * 이를 초기화하고 정리함
 * 프로젝트에서 사용되는 모든 그래픽 객체에 대한 호출을 담당
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 10.02.21
 */
#include "Graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	// 클래스 포인터를 초기화
	m_D3D = 0;				// Direct3D
	m_Camera = 0;			// 카메라
	m_Model = 0;			// 모델
	m_ColorShader = 0;		// 컬러 셰이더
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

// D3DClass 객체를 생성하고 초기화
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D 객체를 생성
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D 객체를 초기화
	result = m_D3D->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D",
			L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// 모델 객체 초기화
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model obj",
			L"Error", MB_OK);
		return false;
	}

	// 컬러 셰이더 객체 생성
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// 컬러 셰이더 객체 초기화
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader obj",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

// 모든 그래픽 obj의 해제가 여기서 일어남
// D3DClass의 정리도 이곳에서
void GraphicsClass::Shutdown()
{
	// 컬러 셰이더 객체를 반환
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// 모델 객체를 반환
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// 카메라 객체를 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// D3D 객체를 반환
	// m_D3D를 null로 초기화했기 때문에
	// null이 아닌 값이라면 초기화
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

// Render 함수를 호출
bool GraphicsClass::Frame()
{
	bool result;

	// 그래픽 렌더링을 수행
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}



bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// D3D 객체에게 화면을 검은색으로 초기화하게 시킴
	// 씬 그리기를 시작히기 위한 버퍼의 내용을 지움
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰, 튜영 행렬을 가져옴
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetviewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여
	// 삼각형 드로잉을 준비함
	m_Model->Render(m_D3D->GetDeviceContext());

	// 색상 셰이더를 사용하여 모델을 렌더링함
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(),
		m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	
	// 버퍼에 그려진씬을 화면에 표시
	m_D3D->EndScene();

	return true;
}