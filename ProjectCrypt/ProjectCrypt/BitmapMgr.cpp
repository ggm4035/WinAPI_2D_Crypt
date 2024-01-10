#include "stdafx.h"
#include "BitmapMgr.h"

CBitmapMgr* CBitmapMgr::m_pInstance = nullptr;

CBitmapMgr::CBitmapMgr()
{
}

CBitmapMgr::~CBitmapMgr()
{
	Release();
}

void CBitmapMgr::Insert_Bmp(const TCHAR * _pFilePath, const TCHAR * _pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), cTag_Finder(_pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBitmap* pBitmap = new CMyBitmap;
		pBitmap->Load_Bmp(_pFilePath);

		m_mapBit.emplace(_pImgKey, pBitmap);
	}
}

HDC CBitmapMgr::Find_Img(const TCHAR * _pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), cTag_Finder(_pImgKey));

	HDC hDC;

	if (!lstrcmp(_pImgKey, iter->first))
		hDC = iter->second->Get_MemDC();

	else
		return nullptr;

	return hDC;
}

void CBitmapMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}
