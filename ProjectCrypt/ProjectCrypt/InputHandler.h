#pragma once
#include "MyCommand.h"

class CInputHandler
{
public:
	CInputHandler();
	~CInputHandler();

public:
	void HandleInput();

private:
	CMyCommand* m_pButton_LEFT;
	CMyCommand* m_pButton_RIGHT;
	CMyCommand* m_pButton_UP;
	CMyCommand* m_pButton_DOWN;
	CMyCommand* m_pButton_X;
	CMyCommand* m_pButton_Z;
	CMyCommand* m_pButton_A;
};

