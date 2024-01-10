#pragma once
#include "Tile.h"

class CTileMgr
{
private:
	static CTileMgr* m_pInstance;
	array<CTile*, TILEX * TILEY> m_arrTile;
	array<vector<CObj*>, TILEX * TILEY> m_arrVector;
	TCHAR* m_pImgKey;
	int m_iChangeTile;

private:
	CTileMgr();
	~CTileMgr();

private:
	void Read_Resource();
	void Clear_Vector();
	vector<CObj*>& Get_Vector(const float& _fX, const float& _fY)
	{
		int iX = int(_fX / TILECX);
		int iY = int(_fY / TILECY);

		return m_arrVector[iY * TILEX + iX];
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

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

	void Picking(const POINT& _pt, int _iDrawX, int _iDrawY);
	void Save_Tiles(const TCHAR* _pFilePath);
	void Load_Tiles(const TCHAR* _pFilePath);
	array<CTile*, TILEX * TILEY>& Get_Tiles() { return m_arrTile; }
	void Set_ImgKey(TCHAR* _pKey) { m_pImgKey = _pKey; }
	void Add_Count() { ++m_iChangeTile; }
	
	void Push_Object(const float& _fX, const float& _fY, CObj* _pObj)
	{
		Get_Vector(_fX, _fY).push_back(_pObj);
	}
	CObj* Pop_Object(const float& _fX, const float& _fY)
	{
		if (Get_Vector(_fX, _fY).empty())
			return nullptr;

		CObj* pObj = Get_Vector(_fX, _fY).back();
		Get_Vector(_fX, _fY).pop_back();

		return pObj;
	}
	CObj* Peek_Object(const float& _fX, const float& _fY)
	{
		if (Get_Vector(_fX, _fY).empty())
			return nullptr;

		return Get_Vector(_fX, _fY).back();
	}
	bool Tile_IsEmpty(const float& _fX, const float& _fY)
	{
		return Get_Vector(_fX, _fY).empty();
	}
};

#define TILE_MGR	CTileMgr::Get_Instance()