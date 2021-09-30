/*
 * GraphicsClass.cpp : SystemClass가 생성하는 또 다른 객체
 * app의 모든 그래픽 기능이 캡술화 되어있음
 * D3DClass를 멤버로 가지고
 * 이를 초기화하고 정리함
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 09.27.21
 */
#include "Graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	// 클래스 포인터를 초기화
	m_D3D = 0;
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

	return true;
}

// 모든 그래픽 obj의 해제가 여기서 일어남
// D3DClass의 정리도 이곳에서
void GraphicsClass::Shutdown()
{
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

// D3D 객체에게 화면을 회색으로 초기화하게 시킴
bool GraphicsClass::Render()
{
	// 씬 그리기를 시작히기 위한 버퍼의 내용을 지움
	m_D3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	// 버퍼에 그려진씬을 화면에 표시
	m_D3D->EndScene();

	return true;
}