#pragma once
/*
 * SystemClass.h : 시스템 클래스
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 09.26.21
 */

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// pre-processing directives
#define WIN32_LEARN_AND_MEAN

// includes
#include <windows.h>

// my class includes
#include "Inputclass.h"
#include "graphicsclass.h"


// systemclass
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitialzeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input = nullptr;
	GraphicsClass* m_Graphics = nullptr;
};

// function prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// globals
static SystemClass* ApplicationHandle = 0;

#endif