/*
 * Dx11Demo_02.cpp : ���� ���α׷��� �������� ����
 * �ۼ��� : 09.25.21
 * ������ ������Ʈ : 09.26.21
 */
#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	PSTR pScmdline,
	int iCmdshow)
{
	// System ��ü ����
	SystemClass* System = new SystemClass;
	bool result;
	
	if (!System)
	{
		return 0;
	}

	// System ��ü �ʱ�ȭ �� run ȣ��
	result = System->Initialize();

	if (result)
	{
		System->Run();
	}

	// System ��ü ���� �� �޸� ��ȯ
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
