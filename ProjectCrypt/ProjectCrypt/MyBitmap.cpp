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
	HDC hDC = GetDC(g_hWnd);	// dc  �Ҵ�ް�
	m_hMemDC = CreateCompatibleDC(hDC);	//�Ҵ���� dc�� �̿��ؼ� ���dc�� ȣȯ�Ǵ�  dc ����

	ReleaseDC(g_hWnd, hDC);	// �ڵ�� �Ҵ���� dc ������

	m_hBitmap = (HBITMAP)LoadImage(NULL, _pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}	

void CMyBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
