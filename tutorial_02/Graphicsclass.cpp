/*
 * GraphicsClass.cpp : SystemClass�� �����ϴ� �� �ٸ� ��ü
 * app�� ��� �׷��� ����� ĸ��ȭ �Ǿ�����
 * ������ ��ũ �ϼ��� ���� ���븸 ����
 * �ۼ��� : 09.25.21
 * ������ ������Ʈ : 09.26.21
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