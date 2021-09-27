/*
 * Dx11Demo_02.cpp : 응용 프로그램의 진입점을 정의
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 09.26.21
 */
#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	PSTR pScmdline,
	int iCmdshow)
{
	// System 객체 생성
	SystemClass* System = new SystemClass;
	bool result;
	
	if (!System)
	{
		return 0;
	}

	// System 객체 초기화 및 run 호출
	result = System->Initialize();

	if (result)
	{
		System->Run();
	}

	// System 객체 종료 및 메모리 반환
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
