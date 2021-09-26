/*
 * GraphicsClass.cpp : SystemClass가 생성하는 또 다른 객체
 * app의 모든 그래픽 기능이 캡술화 되어있음
 * 프레임 워크 완성을 위해 뼈대만 만듬
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 09.26.21
 */
#include "Graphicsclass.h"


GraphicsClass::GraphicsClass()
{

}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	return true;
}

void GraphicsClass::Shutdown()
{
	return;
}

bool GraphicsClass::Frame()
{
	return true;
}

bool GraphicsClass::Render()
{
	return true;
}