/*
 * GraphicsClass.cpp : SystemClass�� �����ϴ� �� �ٸ� ��ü
 * app�� ��� �׷��� ����� ĸ��ȭ �Ǿ�����
 * D3DClass�� ����� ������
 * �̸� �ʱ�ȭ�ϰ� ������
 * �ۼ��� : 09.25.21
 * ������ ������Ʈ : 09.27.21
 */
#include "Graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	// Ŭ���� �����͸� �ʱ�ȭ
	m_D3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

// D3DClass ��ü�� �����ϰ� �ʱ�ȭ
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D ��ü�� ����
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D ��ü�� �ʱ�ȭ
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

// ��� �׷��� obj�� ������ ���⼭ �Ͼ
// D3DClass�� ������ �̰�����
void GraphicsClass::Shutdown()
{
	// D3D ��ü�� ��ȯ
	// m_D3D�� null�� �ʱ�ȭ�߱� ������
	// null�� �ƴ� ���̶�� �ʱ�ȭ
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

// Render �Լ��� ȣ��
bool GraphicsClass::Frame()
{
	bool result;

	// �׷��� �������� ����
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

// D3D ��ü���� ȭ���� ȸ������ �ʱ�ȭ�ϰ� ��Ŵ
bool GraphicsClass::Render()
{
	// �� �׸��⸦ �������� ���� ������ ������ ����
	m_D3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	// ���ۿ� �׷������� ȭ�鿡 ǥ��
	m_D3D->EndScene();

	return true;
}