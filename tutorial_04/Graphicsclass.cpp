/*
 * GraphicsClass.cpp : SystemClass�� �����ϴ� �� �ٸ� ��ü
 * app�� ��� �׷��� ����� ĸ��ȭ �Ǿ�����
 * D3DClass�� ����� ������
 * �̸� �ʱ�ȭ�ϰ� ������
 * ������Ʈ���� ���Ǵ� ��� �׷��� ��ü�� ���� ȣ���� ���
 * �ۼ��� : 09.25.21
 * ������ ������Ʈ : 10.02.21
 */
#include "Graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	// Ŭ���� �����͸� �ʱ�ȭ
	m_D3D = 0;				// Direct3D
	m_Camera = 0;			// ī�޶�
	m_Model = 0;			// ��
	m_ColorShader = 0;		// �÷� ���̴�
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

	// ī�޶� ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ī�޶� ������ ����
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// �� ��ü ����
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// �� ��ü �ʱ�ȭ
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model obj",
			L"Error", MB_OK);
		return false;
	}

	// �÷� ���̴� ��ü ����
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// �÷� ���̴� ��ü �ʱ�ȭ
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader obj",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

// ��� �׷��� obj�� ������ ���⼭ �Ͼ
// D3DClass�� ������ �̰�����
void GraphicsClass::Shutdown()
{
	// �÷� ���̴� ��ü�� ��ȯ
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// �� ��ü�� ��ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// ī�޶� ��ü�� ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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



bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// D3D ��ü���� ȭ���� ���������� �ʱ�ȭ�ϰ� ��Ŵ
	// �� �׸��⸦ �������� ���� ������ ������ ����
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� ����
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, ��, Ʃ�� ����� ������
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetviewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ�
	// �ﰢ�� ������� �غ���
	m_Model->Render(m_D3D->GetDeviceContext());

	// ���� ���̴��� ����Ͽ� ���� ��������
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(),
		m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	
	// ���ۿ� �׷������� ȭ�鿡 ǥ��
	m_D3D->EndScene();

	return true;
}