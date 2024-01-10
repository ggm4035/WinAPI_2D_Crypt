#pragma once
#include "MyBitmap.h"

class CBitmapMgr
{
private:
	static CBitmapMgr* m_pInstance;
	map<const TCHAR*, CMyBitmap*> m_mapBit;

private:
	CBitmapMgr();
	~CBitmapMgr();

public:
	static CBitmapMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBitmapMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void Insert_Bmp(const TCHAR* _pFilePath, const TCHAR* _pImgKey);
	HDC Find_Img(const TCHAR* _pImgKey);
	void Release();
};

#define BITMAP_MGR	CBitmapMgr::Get_Instance()
