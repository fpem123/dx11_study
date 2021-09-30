#pragma once
/*
 * Inputclass.h : Inputclass를 위한 헤더
 * 작성일 : 09.26.21
 * 마지막 업데이트 : 09.26.21
 */
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif