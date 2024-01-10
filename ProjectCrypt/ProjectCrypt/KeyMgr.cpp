#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

// 이전에는 눌림이 없고, 현재 눌렸을 경우
bool CKeyMgr::Key_Down(int _iKey)
{
	if (!m_bKeyState[_iKey] && GetAsyncKeyState(_iKey) & 0x8000)
	{
		m_bKeyState[_iKey] = true;
		return true;
	}

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
		m_bKeyState[_iKey] = false;

	return false;
}

// 이전에 눌림은 있고, 현재 눌리지 않았을 경우
bool CKeyMgr::Key_Up(int _iKey)
{
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
		m_bKeyState[_iKey] = true;

	return false;
}
