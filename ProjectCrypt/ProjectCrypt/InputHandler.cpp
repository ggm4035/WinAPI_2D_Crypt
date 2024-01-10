#include "stdafx.h"
#include "InputHandler.h"
#include "KeyMgr.h"

CInputHandler::CInputHandler()
{
}


CInputHandler::~CInputHandler()
{
}

void CInputHandler::HandleInput()
{
	if (INPUT_MGR->Key_Down(VK_LEFT)) m_pButton_LEFT->execute();
	else if (INPUT_MGR->Key_Down(VK_RIGHT)) m_pButton_RIGHT->execute();
	else if (INPUT_MGR->Key_Down(VK_UP)) m_pButton_UP->execute();
	else if (INPUT_MGR->Key_Down(VK_DOWN)) m_pButton_DOWN->execute();
	else if (INPUT_MGR->Key_Down('Z')) m_pButton_Z->execute();
	else if (INPUT_MGR->Key_Down('X')) m_pButton_X->execute();
	else if (INPUT_MGR->Key_Down('A')) m_pButton_A->execute();
}
