/*
 * stdafx.h : ������ Ű���� �Է��� ó��
 * SystemClass::MessageHandler �Լ��� �Է��� ���� �ް� ���¸� ����
 * ����ִ� ����
 * �ۼ��� : 09.26.21
 * ������ ������Ʈ : 09.26.21
 */
#include "Inputclass.h"


InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{

}

void InputClass::Initialize()
{
	int i;

	// ��� Ű���� ������ ���� ���·� �ʱ�ȭ
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� ����
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// Ű�� �������ٸ� �� ���¸� �迭�� ����
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}