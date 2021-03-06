/*
 * Systemclass.cpp : 
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 09.26.21
 */

#include "systemclass.h"

SystemClass::SystemClass()
{
	// 포인터와 변수를 null로 초기화
	m_Input = 0;
	m_Graphics = 0;
}

// 생성자와 파괴자
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

	// 함수의 높이와 너비를 전달이하기 전에 변수를 초기화
	screenWidth = 0;
	screenHeight = 0;

	// 윈도우즈 api를 사용하여 초기화
	InitialzeWindows(screenWidth, screenHeight);

	// input 객체를 생성, 유저 키입력 처리
	m_Input = new InputClass;

	if (!m_Input)
	{
		return false;
	}

	// input 객체를 초기화
	m_Input->Initialize();

	// graphics 객체를 생성, app의 모든 그래픽 요소를 그림
	m_Graphics = new GraphicsClass;

	if (!m_Graphics)
	{
		return false;
	}

	// graphics 객체를 초기화
	result = m_Graphics->Initialize(screenWidth, screenWidth, m_hwnd);

	if (!result)
	{
		return false;
	}

	return true;
}


// 객체를 정리하는 함수
void SystemClass::Shutdown()
{
	// Graphics 객체를 반환
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input 객체를 반환
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// 창 종료
	ShutdownWindows();

	return;
}

// 프로그램이 종료될 때까지 루프를 돌며 작업을 처리
// App의 모든 작업은 매 루프마다 Frame 함수에서 수행
// while 종료되지 않는 동안
//		윈도우의 시스템 메시지를 확인
//		메세지 처리
//		app의 작업
//		유저가 작업중 종료를 원하는지 확인
void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// 메세지 구조체를 초기화
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료 메시지를 받을 때까지 루프
	done = false;

	while (!done)
	{
		// 윈도우 메시지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// app 종료 요청하는 경우 빠져나감
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 그 외엔 Frame 함수 처리
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

// App의 모든 작업이 처리되는 곳
bool SystemClass::Frame()
{
	bool result;

	// 유저가 ESC키를 눌러 app을종료하길 원하는 지 확인
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics 객체의 작업을 처리
	result = m_Graphics->Frame();

	if (!result)
	{
		return false;
	}

	return true;
}

// 윈도우의 시스템 메시지가 전달되는 곳
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// 키다운이 있는지 확인
		case WM_KEYDOWN:
		{
			// 키다운이 있다면 input 객체에 전달하여 기록
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// 키보드의 눌린 키가 떼어졌는지 확인
		case WM_KEYUP:
		{
			// 키가 떼졌다면 input 객체에 전달하여 기록
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// 다른 메시지들은 사용하지 않으므로 기본 메시지 처리기에 전달
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

// 렌더링 하게 될 윈도우를 만드는 코드
void SystemClass::InitialzeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부 포인터를 이 객체로 설정
	ApplicationHandle = this;

	// 이 app의 인스턴스를 가져옴
	m_hinstance = GetModuleHandle(NULL);

	// app의 이름을 설정
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춤
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

	// 윈도우 클래스를 등록
	RegisterClassEx(&wc);

	// 모니터의 화면 해상도를 얻어옴
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CXSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면을 설정
	if (FULL_SCREEN)
	{
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면의 왼쪽위로
		posX = posY = 0;
	}
	else
	{
		// 윈도우 모드라면 800x600
		screenWidth = 800;
		screenHeight = 600;

		// 창을 모니터 중앙에 오게
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CXSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들고 핸들을 가져옴
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 줌
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않습니다
	ShowCursor(false);

	return;
}

// 화면 설정을 되돌리고 윈도우와 핸들들을 반환
void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿈
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// app 인스턴스를 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거
	ApplicationHandle = NULL;

	return;
}

// 윈도우시스템에서 메시지를 보내는 곳
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우가 제거되었는지 확인
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// 윈도우가 닫히는지 확인
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// 다른 모든 메시지들은 system 클래스의 메시지 처리기에 전달
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}