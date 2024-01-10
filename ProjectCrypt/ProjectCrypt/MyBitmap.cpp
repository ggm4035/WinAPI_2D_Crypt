#include "stdafx.h"
#include "MyBitmap.h"

CMyBitmap::CMyBitmap()
{
}

CMyBitmap::~CMyBitmap()
{
	Release();
}

void CMyBitmap::Load_Bmp(const TCHAR * _pFilePath)
{
	HDC hDC = GetDC(g_hWnd);	// dc  할당받고
	m_hMemDC = CreateCompatibleDC(hDC);	//할당받은 dc를 이용해서 멤버dc에 호환되는  dc 생성

	ReleaseDC(g_hWnd, hDC);	// 핸들로 할당받은 dc 릴리즈

	m_hBitmap = (HBITMAP)LoadImage(NULL, _pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}	

void CMyBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
