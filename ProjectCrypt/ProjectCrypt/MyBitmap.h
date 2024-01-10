#pragma once
#include "Define.h"

class CMyBitmap
{
private:
	HDC m_hMemDC;

	HBITMAP m_hBitmap;
	HBITMAP m_hOldmap;

public:
	CMyBitmap();
	~CMyBitmap();

public:
	HDC Get_MemDC() { return m_hMemDC; }
	void Load_Bmp(const TCHAR* _pFilePath);
	void Release();
};

