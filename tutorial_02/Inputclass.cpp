/*
 * stdafx.h : 유저의 키보드 입력을 처리
 * SystemClass::MessageHandler 함수로 입력을 전달 받고 상태를 저장
 * 들어있는 파일
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 09.26.21
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

	// 모든 키들을 눌리지 않은 상태로 초기화
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// 키가 떼어졌다면 그 상태를 배열에 저장
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}