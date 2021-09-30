/*
 * Systemclass.cpp : 
 * �ۼ��� : 09.26.21
 * ������ ������Ʈ : 09.26.21
 */

#include "systemclass.h"

SystemClass::SystemClass()
{
	// �����Ϳ� ������ null�� �ʱ�ȭ
	m_Input = 0;
	m_Graphics = 0;
}

// �����ڿ� �ı���
SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// �Լ��� ���̿� �ʺ� �������ϱ� ���� ������ �ʱ�ȭ
	screenWidth = 0;
	screenHeight = 0;

	// �������� api�� ����Ͽ� �ʱ�ȭ
	InitialzeWindows(screenWidth, screenHeight);

	// input ��ü�� ����, ���� Ű�Է� ó��
	m_Input = new InputClass;

	if (!m_Input)
	{
		return false;
	}

	// input ��ü�� �ʱ�ȭ
	m_Input->Initialize();

	// graphics ��ü�� ����, app�� ��� �׷��� ��Ҹ� �׸�
	m_Graphics = new GraphicsClass;

	if (!m_Graphics)
	{
		return false;
	}

	// graphics ��ü�� �ʱ�ȭ
	result = m_Graphics->Initialize(screenWidth, screenWidth, m_hwnd);

	if (!result)
	{
		return false;
	}

	return true;
}


// ��ü�� �����ϴ� �Լ�
void SystemClass::Shutdown()
{
	// Graphics ��ü�� ��ȯ
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input ��ü�� ��ȯ
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// â ����
	ShutdownWindows();

	return;
}

// ���α׷��� ����� ������ ������ ���� �۾��� ó��
// App�� ��� �۾��� �� �������� Frame �Լ����� ����
// while ������� �ʴ� ����
//		�������� �ý��� �޽����� Ȯ��
//		�޼��� ó��
//		app�� �۾�
//		������ �۾��� ���Ḧ ���ϴ��� Ȯ��
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// �޼��� ����ü�� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޽����� ���� ������ ����
	done = false;

	while (!done)
	{
		// ������ �޽��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// app ���� ��û�ϴ� ��� ��������
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �ܿ� Frame �Լ� ó��
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

// App�� ��� �۾��� ó���Ǵ� ��
bool SystemClass::Frame()
{
	bool result;

	// ������ ESCŰ�� ���� app�������ϱ� ���ϴ� �� Ȯ��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics ��ü�� �۾��� ó��
	result = m_Graphics->Frame();

	if (!result)
	{
		return false;
	}

	return true;
}

// �������� �ý��� �޽����� ���޵Ǵ� ��
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Ű�ٿ��� �ִ��� Ȯ��
		case WM_KEYDOWN:
		{
			// Ű�ٿ��� �ִٸ� input ��ü�� �����Ͽ� ���
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Ű������ ���� Ű�� ���������� Ȯ��
		case WM_KEYUP:
		{
			// Ű�� �����ٸ� input ��ü�� �����Ͽ� ���
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// �ٸ� �޽������� ������� �����Ƿ� �⺻ �޽��� ó���⿡ ����
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

// ������ �ϰ� �� �����츦 ����� �ڵ�
void SystemClass::InitialzeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;

	// �� app�� �ν��Ͻ��� ������
	m_hinstance = GetModuleHandle(NULL);

	// app�� �̸��� ����
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ������ ���
	RegisterClassEx(&wc);

	// ������� ȭ�� �ػ󵵸� ����
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CXSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ���� ����
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit��
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ���� ��������
		posX = posY = 0;
	}
	else
	{
		// ������ ����� 800x600
		screenWidth = 800;
		screenHeight = 600;

		// â�� ����� �߾ӿ� ����
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CXSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �ڵ��� ������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� ��
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ �ʽ��ϴ�
	ShowCursor(false);

	return;
}

// ȭ�� ������ �ǵ����� ������� �ڵ���� ��ȯ
void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ��
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲ�
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// app �ν��Ͻ��� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ ����
	ApplicationHandle = NULL;

	return;
}

// ������ý��ۿ��� �޽����� ������ ��
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// �����찡 ���ŵǾ����� Ȯ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �����찡 �������� Ȯ��
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// �ٸ� ��� �޽������� system Ŭ������ �޽��� ó���⿡ ����
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}